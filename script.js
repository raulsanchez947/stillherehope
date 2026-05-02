const chatMessages = document.querySelector("#chatMessages");
const chatForm = document.querySelector("#chatForm");
const chatInput = document.querySelector("#chatInput");
const quickPrompts = document.querySelectorAll("[data-prompt]");
const sparkForm = document.querySelector("#sparkForm");
const sparkInput = document.querySelector("#sparkInput");
const sparkWall = document.querySelector("#sparkWall");
const chatModeLabel = document.querySelector("#chatModeLabel");
const chatStatusDot = document.querySelector("#chatStatusDot");
const chatNotice = document.querySelector("#chatNotice");

const chatHistory = [];
let siteStatus = {
  aiEnabled: false
};

const starterSparks = [
  "You do not need to solve your whole life tonight. Just stay with the next breath.",
  "Someone else has felt this heavy and still found a morning that surprised them.",
  "Rest is not quitting. Rest is how you keep going.",
  "If all you did today was survive, that still counts as courage.",
  "Your story can have a softer chapter after this one.",
  "Ask for help before you believe you have to disappear."
];

const responseBank = [
  {
    keywords: ["anxious", "panic", "overwhelmed", "scared", "stress"],
    reply:
      "Let's make the room smaller for a moment. Unclench your jaw, drop your shoulders, and try this: inhale for 4, hold for 2, exhale for 6. Do that three times. You do not need to defeat the whole feeling. We are just helping your body notice that this second is survivable."
  },
  {
    keywords: ["sad", "depressed", "hopeless", "empty", "cry"],
    reply:
      'I am really sorry it feels this dim right now. Please try one tiny anchor: drink water, sit near a light, or text one safe person, "Can you sit with me for a bit?" You are not a burden for needing care. If you might hurt yourself, call or text 988 now in the U.S. or contact local emergency help.'
  },
  {
    keywords: ["alone", "lonely", "no one", "isolated"],
    reply:
      "Loneliness can make the world feel like a locked door. Let's look for one handle: name one person, place, community, pet, memory, song, or value that has ever made you feel even 1% connected. That small thread still matters, and it can be enough to reach for the next thread."
  },
  {
    keywords: ["angry", "mad", "rage", "unfair"],
    reply:
      "Anger often shows up to protect something tender. Before you act, try pressing both feet into the floor and naming the thing underneath the anger: hurt, fear, grief, disrespect, exhaustion, or love. Then choose the next action that protects your future self too."
  },
  {
    keywords: ["ground", "breathe", "calm", "spiral"],
    reply:
      'Try the 5-4-3-2-1 reset: name 5 things you can see, 4 things you can feel, 3 sounds, 2 things you can smell, and 1 thing you can taste. Then say: "I am here. This is a feeling. Feelings move."'
  },
  {
    keywords: ["give up", "suicide", "kill myself", "end it", "hurt myself", "can't go on"],
    reply:
      "I am glad you said something. Please do not stay alone with this. If you are in the U.S., call or text 988 right now, or use 988lifeline.org to chat with a trained crisis counselor. If you are in immediate danger, call emergency services or go to the nearest emergency room. While help is connecting, move away from anything you could use to hurt yourself and get near another person if you can."
  }
];

function addMessage(text, sender) {
  const message = document.createElement("article");
  message.className = `message ${sender}`;

  const body = document.createElement("p");
  body.textContent = text;
  message.appendChild(body);

  chatMessages.appendChild(message);
  chatMessages.scrollTop = chatMessages.scrollHeight;
}

function setChatMode(mode, detail) {
  if (mode === "ai") {
    chatModeLabel.textContent = "AI support is live";
    chatStatusDot.textContent = "AI live";
    chatNotice.textContent =
      "You are in a guided support conversation. If things feel urgent or unsafe, use 988 or local emergency support right away.";
    return;
  }

  if (mode === "fallback") {
    chatModeLabel.textContent = "Local support mode";
    chatStatusDot.textContent = "Fallback";
    chatNotice.textContent =
      detail ||
      "AI is not configured yet, so the guide is using local calming responses on this device.";
    return;
  }

  chatModeLabel.textContent = "Checking AI connection...";
  chatStatusDot.textContent = "Starting";
}

async function fetchStatus() {
  try {
    const response = await fetch("/api/status", { cache: "no-store" });

    if (!response.ok) {
      throw new Error("Status unavailable");
    }

    siteStatus = await response.json();
    setChatMode(siteStatus.aiEnabled ? "ai" : "fallback");
  } catch (error) {
    setChatMode(
      "fallback",
      "AI becomes available when you run the local Node server and add an OpenAI API key."
    );
  }
}

function getGuideReply(message) {
  const normalized = message.toLowerCase();
  const match = responseBank.find((entry) =>
    entry.keywords.some((keyword) => normalized.includes(keyword))
  );

  if (match) {
    return match.reply;
  }

  return "Thank you for trusting this space with that. Let's make the next step gentle: name what you feel, name what you need, and choose one small action that would make the next ten minutes safer or softer. If this is urgent or you may be in danger, please contact emergency services or 988 in the U.S.";
}

async function getChatReply(message) {
  try {
    const response = await fetch("/api/chat", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({
        message,
        history: chatHistory.slice(-8)
      })
    });

    if (!response.ok) {
      throw new Error("Chat unavailable");
    }

    const data = await response.json();
    setChatMode(data.mode === "ai" ? "ai" : "fallback");
    return data.reply || getGuideReply(message);
  } catch (error) {
    setChatMode(
      "fallback",
      "The AI server is not reachable right now, so the guide is using local support mode."
    );
    return getGuideReply(message);
  }
}

async function handleChatSubmit(message) {
  const trimmed = message.trim();

  if (!trimmed) {
    return;
  }

  addMessage(trimmed, "user");
  chatHistory.push({ role: "user", content: trimmed });
  chatInput.value = "";
  chatInput.disabled = true;

  const reply = await getChatReply(trimmed);
  addMessage(reply, "guide");
  chatHistory.push({ role: "assistant", content: reply });
  chatInput.disabled = false;
  chatInput.focus();
}

function loadSparks() {
  const saved = JSON.parse(localStorage.getItem("hope-sparks") || "null");
  return Array.isArray(saved) && saved.length > 0 ? saved : starterSparks;
}

function saveSparks(sparks) {
  localStorage.setItem("hope-sparks", JSON.stringify(sparks));
}

function renderSparks() {
  const sparks = loadSparks();
  sparkWall.innerHTML = "";

  sparks.slice(0, 12).forEach((spark) => {
    const card = document.createElement("article");
    card.className = "spark-card";
    card.textContent = spark;
    sparkWall.appendChild(card);
  });
}

chatForm.addEventListener("submit", async (event) => {
  event.preventDefault();
  await handleChatSubmit(chatInput.value);
});

quickPrompts.forEach((button) => {
  button.addEventListener("click", async () => {
    await handleChatSubmit(button.dataset.prompt);
  });
});

sparkForm.addEventListener("submit", (event) => {
  event.preventDefault();

  const spark = sparkInput.value.trim();

  if (!spark) {
    return;
  }

  const updatedSparks = [spark, ...loadSparks()].slice(0, 12);
  saveSparks(updatedSparks);
  sparkInput.value = "";
  renderSparks();
});

addMessage(
  "I am here with you. Tell me what feels heaviest, or tap a quick prompt. We will look for one calmer next step together.",
  "guide"
);

chatHistory.push({
  role: "assistant",
  content:
    "I am here with you. Tell me what feels heaviest, or tap a quick prompt. We will look for one calmer next step together."
});

renderSparks();
fetchStatus();
