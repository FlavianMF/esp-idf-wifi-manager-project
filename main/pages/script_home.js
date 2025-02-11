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

async function fetchChipId() {
    fetch('/chip_id')
        .then(response => {
            if (!response.ok) {
                throw new Error("Falha ao obter Chip ID (HTTP " + response.status + ")");
            }
            return response.text();
        })
        .then(chipId => {
            console.log("Chip ID:", chipId);
            document.getElementById("chip-id").textContent = "Chip ID: " + chipId;
        })
        .catch(error => {
            console.error("Erro ao obter o Chip ID:", error);
            document.getElementById("chip-id").textContent = "Chip ID: Erro";
            document.getElementById("chip-id").style.color = "red"; // Destaca visualmente o erro
        });
}

async function fetchBuildInfo() {
    fetch('/app_version')
        .then(response => {
            if (!response.ok) {
                throw new Error("Falha ao obter Informações de Build (HTTP " + response.status + ")");
            }
            return response.json();
        })
        .then(buildInfo => {
            console.log("Informações de Build:", buildInfo);
            document.getElementById("build-version").textContent = "App Version: " + buildInfo.app_version;
            document.getElementById("build-time").textContent = "Build: " + buildInfo.build_date + " | " + buildInfo.build_time;
        })
        .catch(error => {
            console.error("Erro ao obter Informações de Build:", error);
            document.getElementById("build-version").textContent = "Versão: Erro";
            document.getElementById("build-time").textContent = "Build: Erro";
            document.getElementById("build-version").style.color = "red"; // Destaca visualmente o erro
            document.getElementById("build-time").style.color = "red"; // Destaca visualmente o erro
        });
}

window.onload = async function () {
    await fetchChipId();
    await fetchBuildInfo();
};