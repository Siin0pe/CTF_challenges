(() => {
  const encodedFlag = [70, 76, 65, 71, 95, 76, 79, 67, 65, 76, 95, 83, 84, 79, 82, 65, 71, 69, 95, 71, 65, 68, 71, 69, 84];
  const flag = String.fromCharCode(...encodedFlag);
  const noise = {
    drift: Math.random().toFixed(4),
    cache: "volatile",
    timestamp: new Date().toISOString(),
  };

  localStorage.setItem("bonusA_cache", flag);
  localStorage.setItem("bonusA_noise", JSON.stringify(noise));

  console.log("%c[SCAN MEMOIRE]", "color:#f97316;font-weight:700", "Bloc suspect écrit dans le stockage local.");
  console.log("%c[Alerte console]", "color:#f43f5e;font-weight:700", "Un fragment masqué fuit côté navigateur.");
})();
