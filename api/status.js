module.exports = function handler(_request, response) {
  response.setHeader("Cache-Control", "no-store");
  response.status(200).json({
    aiEnabled: Boolean(process.env.OPENAI_API_KEY),
    chatEndpoint: "/api/chat"
  });
};
