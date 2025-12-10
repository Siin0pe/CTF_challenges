const output = document.getElementById("output");
const answerEl = document.getElementById("answer");
const submitBtn = document.getElementById("submit");
const secret = "secret.html";

let expectedAnswer = 0;

function render(text) {
  if (output) {
    output.textContent = text;
  }
}

function generateCaptcha() {
  const num1 = Math.floor(Math.random() * 10) + 1;
  const num2 = Math.floor(Math.random() * 10) + 1;

  expectedAnswer = num1 + num2;

  const label = document.querySelector("label[for='answer']");
  if (label) {
    label.textContent = `${num1} + ${num2} = ?`;
  }
}

function verifyCaptcha() {
  const answer = answerEl ? answerEl.value : "";
  if (answer === expectedAnswer) {
    window.location.href = secret;
  } else {
    render("Réponse incorrecte — vérifiez votre saisie.");
  }
}

document.addEventListener("DOMContentLoaded", () => {
  generateCaptcha();

  if (submitBtn) {
    submitBtn.addEventListener("click", verifyCaptcha);
  }
});
