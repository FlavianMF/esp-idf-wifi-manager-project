// Simula valores do microcontrolador (substitua com valores reais via JavaScript)
document.getElementById("chip-id").textContent = "Chip ID: 12345678";
document.getElementById("build-version").textContent = "Versão: 1.0.0";
document.getElementById("build-time").textContent = "Build: 10/02/2025 14:30";

function salvarConfiguracao() {
    const ssid = document.getElementById("wifi-ssid").value;
    const senha = document.getElementById("wifi-password").value;
    const modo = document.getElementById("mode").value;

    // Simula envio para o microcontrolador
    console.log("Salvando Configuração...");
    console.log("SSID:", ssid);
    console.log("Senha:", senha);
    console.log("Modo:", modo);

    alert("Configuração salva com sucesso!");
}

window.onload = function () {
    const ssid = document.getElementById("wifi-ssid").value;
    const senha = document.getElementById("wifi-password").value;
    const modo = document.getElementById("mode").value;
};