import React, { useMemo, useState } from "react";
import { createRoot } from "react-dom/client";
import {
  BookOpenText,
  Flame,
  Flower2,
  HeartHandshake,
  RefreshCw,
  ShieldCheck,
  Sparkles
} from "lucide-react";
import "./styles.css";

const tabs = [
  { id: "chakras", label: "Chakras", icon: Flower2 },
  { id: "archangels", label: "Angels", icon: ShieldCheck },
  { id: "affirmations", label: "Affirm", icon: HeartHandshake },
  { id: "sessions", label: "Sessions", icon: Sparkles }
];

const chakraData = [
  {
    name: "Root",
    sanskrit: "Muladhara",
    location: "Base of spine",
    symbol: "",
    color: "#c94a4a",
    element: "Earth",
    mantra: "I am safe, rooted, and supported.",
    blocked: "Fear, scarcity thoughts, instability, or feeling disconnected from the body.",
    balanced: "Grounded, secure, steady, practical, and able to trust the present moment.",
    prompt: "Where do I need to feel safer before I can grow?"
  },
  {
    name: "Sacral",
    sanskrit: "Svadhisthana",
    location: "Below the navel",
    symbol: "",
    color: "#e98a3e",
    element: "Water",
    mantra: "My feelings are welcome. My creativity is alive.",
    blocked: "Shame, creative numbness, guilt around pleasure, or emotional shutdown.",
    balanced: "Creative, emotionally fluid, sensual, playful, and comfortable receiving joy.",
    prompt: "What part of me wants permission to feel and create?"
  },
  {
    name: "Solar Plexus",
    sanskrit: "Manipura",
    location: "Upper abdomen",
    symbol: "",
    color: "#f2bf3d",
    element: "Fire",
    mantra: "I choose with confidence and honor my power.",
    blocked: "Self-doubt, people pleasing, low motivation, or fear of being seen.",
    balanced: "Confident, disciplined, energized, decisive, and able to act with integrity.",
    prompt: "Where have I made myself smaller to stay accepted?"
  },
  {
    name: "Heart",
    sanskrit: "Anahata",
    location: "Center of chest",
    symbol: "heart",
    color: "#5fbf86",
    element: "Air",
    mantra: "I give and receive love with wise boundaries.",
    blocked: "Grief, resentment, guardedness, loneliness, or difficulty forgiving.",
    balanced: "Compassionate, connected, forgiving, discerning, and open to love.",
    prompt: "What would love say if it also protected my peace?"
  },
  {
    name: "Throat",
    sanskrit: "Vishuddha",
    location: "Throat",
    symbol: "",
    color: "#55a6d9",
    element: "Ether",
    mantra: "My voice is clear, honest, and kind.",
    blocked: "Suppressed truth, fear of conflict, gossip loops, or trouble asking for needs.",
    balanced: "Authentic, expressive, thoughtful, and able to listen as well as speak.",
    prompt: "What truth have I been swallowing?"
  },
  {
    name: "Third Eye",
    sanskrit: "Ajna",
    location: "Between the eyebrows",
    symbol: "",
    color: "#7355c8",
    element: "Light",
    mantra: "I trust my inner knowing and seek clarity.",
    blocked: "Confusion, overthinking, ignoring intuition, or clinging to illusion.",
    balanced: "Intuitive, imaginative, perceptive, reflective, and mentally clear.",
    prompt: "What pattern am I ready to see clearly?"
  },
  {
    name: "Crown",
    sanskrit: "Sahasrara",
    location: "Top of the head",
    symbol: "crown",
    color: "#b78be8",
    element: "Spirit",
    mantra: "I am connected to wisdom greater than fear.",
    blocked: "Spiritual disconnection, cynicism, isolation, or lack of meaning.",
    balanced: "Peaceful, connected, purposeful, humble, and aligned with higher guidance.",
    prompt: "What belief helps me feel connected to something larger?"
  }
];

const archangels = [
  {
    name: "Michael",
    day: "Sunday",
    role: "Protection",
    color: "#4d7bd9",
    description:
      "Call on Michael when you need courage, energetic clearing, boundaries, or protection before hard conversations."
  },
  {
    name: "Gabriel",
    day: "Monday",
    role: "Messages",
    color: "#f2f5ff",
    description:
      "Call on Gabriel for clarity, creative communication, dreams, announcements, writing, and honest expression."
  },
  {
    name: "Raphael",
    day: "Wednesday",
    role: "Healing",
    color: "#6fc58a",
    description:
      "Call on Raphael for emotional, physical, or relational healing, especially when you need gentleness and renewal."
  },
  {
    name: "Chamuel",
    day: "Friday",
    role: "Love",
    color: "#ef8cb7",
    description:
      "Call on Chamuel for self-love, forgiveness, relationship harmony, inner child care, and finding what was lost."
  }
];

const affirmations = [
  "I am allowed to heal without rushing.",
  "My energy is precious, and I choose where it flows.",
  "I can be soft and powerful at the same time.",
  "I release what is not mine to carry.",
  "My body is a safe home for my spirit.",
  "Love meets me in honest, grounded ways.",
  "I trust the timing of my becoming.",
  "My intuition speaks clearly when I get quiet.",
  "I am protected, guided, and deeply supported.",
  "I can begin again with grace.",
  "I honor my shadow without letting it drive.",
  "The cure within me begins with listening."
];

const healingSessions = [
  {
    name: "Ground + Return",
    length: "4 minutes",
    intention: "For anxiety, overwhelm, or scattered energy.",
    steps: ["Hand on heart, feet on floor.", "Breathe in for 4, out for 6.", "Name one thing that is safe right now."],
    prompt: "What would help my body feel 5% safer today?"
  },
  {
    name: "Inner Child Check-In",
    length: "7 minutes",
    intention: "For tenderness, old wounds, or needing comfort.",
    steps: ["Picture yourself at a younger age.", "Ask what they needed to hear.", "Write the answer without correcting it."],
    prompt: "Little me, what are you still waiting for me to understand?"
  },
  {
    name: "Shadow Softening",
    length: "9 minutes",
    intention: "For triggers, jealousy, resentment, or repeating patterns.",
    steps: ["Name the feeling without judging it.", "Ask what it is protecting.", "Choose one honest next step."],
    prompt: "What part of me am I ready to stop rejecting?"
  },
  {
    name: "Call In Support",
    length: "5 minutes",
    intention: "For protection, love, clarity, or healing guidance.",
    steps: ["Choose an archangel or loving guide.", "State the support you are asking for.", "Close with gratitude and one grounded action."],
    prompt: "What support am I finally willing to receive?"
  }
];

const sourceLinks = [
  ["Cleveland Clinic: chakras overview", "https://health.clevelandclinic.org/chakras"],
  ["Britannica: archangel", "https://www.britannica.com/topic/archangel"],
  ["NIH NCCIH: meditation and mindfulness", "https://www.nccih.nih.gov/health/meditation-and-mindfulness-effectiveness-and-safety"]
];

function App() {
  const [activeTab, setActiveTab] = useState("chakras");
  const [openChakra, setOpenChakra] = useState("");
  const [affirmationIndex, setAffirmationIndex] = useState(0);
  const [journalText, setJournalText] = useState(() => localStorage.getItem("energetic-journal") || "");
  const [burned, setBurned] = useState(false);

  const activeTitle = useMemo(() => tabs.find((tab) => tab.id === activeTab)?.label, [activeTab]);

  function updateJournal(value) {
    setJournalText(value);
    localStorage.setItem("energetic-journal", value);
    setBurned(false);
  }

  function burnJournal() {
    setJournalText("");
    localStorage.removeItem("energetic-journal");
    setBurned(true);
  }

  return (
    <main className="app-shell">
      <header className="hero">
        <div className="hero-glow" aria-hidden="true" />
        <p className="brand-kicker">Energetic Empire</p>
        <h1>Astra Lux</h1>
        <p className="tagline">Love · Vibes · Healing</p>
        <div className="program-pill">
          <Sparkles size={16} />
          The Cure Within starter guide
        </div>
      </header>

      <section className="content-panel" aria-label={activeTitle}>
        {activeTab === "chakras" && (
          <Chakras openChakra={openChakra} setOpenChakra={setOpenChakra} />
        )}
        {activeTab === "archangels" && <Archangels />}
        {activeTab === "affirmations" && (
          <Affirmations
            affirmationIndex={affirmationIndex}
            setAffirmationIndex={setAffirmationIndex}
          />
        )}
        {activeTab === "sessions" && <HealingSessions />}

        <Journal
          text={journalText}
          updateText={updateJournal}
          burnJournal={burnJournal}
          burned={burned}
        />
        <Sources />
      </section>

      <nav className="tab-bar" aria-label="Main sections">
        {tabs.map((tab) => {
          const Icon = tab.icon;
          return (
            <button
              className={activeTab === tab.id ? "active" : ""}
              key={tab.id}
              type="button"
              onClick={() => setActiveTab(tab.id)}
              aria-current={activeTab === tab.id ? "page" : undefined}
            >
              <Icon size={20} />
              <span>{tab.label}</span>
            </button>
          );
        })}
      </nav>
    </main>
  );
}

function SectionIntro({ eyebrow, title, children }) {
  return (
    <div className="section-intro">
      <p>{eyebrow}</p>
      <h2>{title}</h2>
      {children && <span>{children}</span>}
    </div>
  );
}

function Chakras({ openChakra, setOpenChakra }) {
  return (
    <>
      <h3 className="chakra-heading">The 7 Chakras</h3>
      <div className="chakra-grid">
        {chakraData.map((chakra) => {
          const isOpen = openChakra === chakra.name;
          return (
            <article
              className={`chakra-card ${isOpen ? "expanded" : ""}`}
              key={chakra.name}
              style={{ "--chakra": chakra.color }}
            >
              <button type="button" onClick={() => setOpenChakra(isOpen ? "" : chakra.name)}>
                <span className="chakra-orb" style={{ "--chakra": chakra.color }}>
                  {chakra.symbol === "heart" && <span>♥</span>}
                  {chakra.symbol === "crown" && <span>♛</span>}
                </span>
                <span className="chakra-copy">
                  <strong>{chakra.name} Chakra</strong>
                  <small>
                    {chakra.sanskrit} · {chakra.location}
                  </small>
                </span>
                <span className={`chakra-chevron ${isOpen ? "open" : ""}`}>▾</span>
              </button>
              {isOpen && (
                <div className="chakra-details">
                  <Detail label="Element" value={chakra.element} />
                  <Detail label="Mantra" value={chakra.mantra} />
                  <Detail label="When blocked" value={chakra.blocked} />
                  <Detail label="When balanced" value={chakra.balanced} />
                  <Detail label="Shadow prompt" value={chakra.prompt} />
                </div>
              )}
            </article>
          );
        })}
      </div>
    </>
  );
}

function Detail({ label, value }) {
  return (
    <p>
      <b>{label}:</b> {value}
    </p>
  );
}

function Archangels() {
  return (
    <>
      <SectionIntro eyebrow="Guides by intention" title="Call on the energy that matches the need.">
        Days are included as devotional associations, not rules.
      </SectionIntro>
      <div className="angel-list">
        {archangels.map((angel) => (
          <article className="angel-card" key={angel.name}>
            <span className="angel-light" style={{ "--angel": angel.color }} />
            <div>
              <p>{angel.day}</p>
              <h3>Archangel {angel.name}</h3>
              <strong>{angel.role}</strong>
              <span>{angel.description}</span>
            </div>
          </article>
        ))}
      </div>
    </>
  );
}

function Affirmations({ affirmationIndex, setAffirmationIndex }) {
  const affirmation = affirmations[affirmationIndex];

  return (
    <>
      <SectionIntro eyebrow="Daily reset" title="One affirmation at a time.">
        Tap when your energy asks for a new sentence to hold.
      </SectionIntro>
      <article className="affirmation-card">
        <p>{affirmation}</p>
        <button
          type="button"
          onClick={() => setAffirmationIndex((affirmationIndex + 1) % affirmations.length)}
        >
          <RefreshCw size={18} />
          New affirmation
        </button>
      </article>
      <article className="quote-card">
        <BookOpenText size={20} />
        <p>
          "Everything is energy and that's all there is to it. Match the frequency of the
          reality you want..."
        </p>
        <span>
          Often attributed to Albert Einstein online; included as a reflection prompt, not a
          verified Einstein quotation.
        </span>
      </article>
    </>
  );
}

function HealingSessions() {
  return (
    <>
      <SectionIntro eyebrow="Prompted healing" title="Choose a guided session and write what comes up.">
        These are soft self-reflection flows that pair naturally with the private note below.
      </SectionIntro>
      <div className="session-list">
        {healingSessions.map((session) => (
          <article className="session-card" key={session.name}>
            <div>
              <strong>{session.length}</strong>
              <h3>{session.name}</h3>
              <p>{session.intention}</p>
            </div>
            <ol>
              {session.steps.map((step) => (
                <li key={step}>{step}</li>
              ))}
            </ol>
            <p className="session-prompt">{session.prompt}</p>
          </article>
        ))}
      </div>
    </>
  );
}

function Journal({ text, updateText, burnJournal, burned }) {
  return (
    <aside className="journal-card">
      <div>
        <p className="mini-kicker">Private reflection</p>
        <h2>Write, release, or burn.</h2>
        <span>
          This prototype stores writing only in this browser's local storage. There is no
          account, server save, or admin view connected.
        </span>
      </div>
      <label htmlFor="journal">Free write, shadow work, or inner child note</label>
      <textarea
        id="journal"
        value={text}
        onChange={(event) => updateText(event.target.value)}
        placeholder="What is my energy trying to tell me today?"
      />
      <div className="journal-actions">
        <button type="button" onClick={burnJournal}>
          <Flame size={18} />
          Burn note
        </button>
        {burned && <span>Your note was cleared from this browser.</span>}
      </div>
    </aside>
  );
}

function Sources() {
  return (
    <aside className="source-card">
      <p className="mini-kicker">Research + disclaimer</p>
      <p>
        Energetic practices are personal and cultural. Do your own research, confirm what
        resonates, and use professional medical or mental health support when needed.
      </p>
      <div className="source-links">
        {sourceLinks.map(([label, href]) => (
          <a href={href} target="_blank" rel="noreferrer" key={href}>
            {label}
          </a>
        ))}
      </div>
      <p className="future-note">
        Future companion app idea: crystals, herbs, sound frequencies, image capture, and a
        deeper reference library can live together instead of overloading this first app.
      </p>
    </aside>
  );
}

createRoot(document.getElementById("root")).render(<App />);
