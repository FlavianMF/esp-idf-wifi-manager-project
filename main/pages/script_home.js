async function fetchResetBoard() {
    fetch('/reset_board')
    .then(response => {
        if (!response.ok) {
            throw new Error("Falha ao resetar o microcontrolador (HTTP " + response.status + ")");
        }
        return response.text();
    })
    .then(responseText => {
        console.log("Resposta do servidor:", responseText);

        alert("Microcontrolador resetado com sucesso!");
        
        window.location.reload();
    })
    .catch(error => {
        console.error(error);
        alert(error.message);
    })
}


async function fetchResetReason() {
    fetch('/reset_reason')
        .then(response => {
            if (!response.ok) {
                throw new Error("Falha ao obter Informações de Reset (HTTP " + response.status + ")");
            }
            return response.text();
        })
        .then(resetReason => {
            console.log("Informações de Reset:", resetReason);

            const resetReasons = [
                "Reset reason can not be determined",                       // ESP_RST_UNKNOWN
                "Reset due to power-on event",                              // ESP_RST_POWERON
                "Reset by external pin (not applicable for ESP32)",         // ESP_RST_EXT
                "Software reset via esp_restart",                           // ESP_RST_SW
                "Software reset due to exception/panic",                    // ESP_RST_PANIC
                "Reset (software or hardware) due to interrupt watchdog",   // ESP_RST_INT_WDT
                "Reset due to task watchdog",                               // ESP_RST_TASK_WDT
                "Reset due to other watchdogs",                             // ESP_RST_WDT
                "Reset after exiting deep sleep mode",                      // ESP_RST_DEEPSLEEP
                "Brownout reset (software or hardware)",                    // ESP_RST_BROWNOUT
                "Reset over SDIO",                                          // ESP_RST_SDIO
                "Reset by USB peripheral",                                  // ESP_RST_USB
                "Reset by JTAG",                                            // ESP_RST_JTAG
                "Reset due to efuse error",                                 // ESP_RST_EFUSE
                "Reset due to power glitch detected",                       // ESP_RST_PWR_GLITCH
                "Reset due to CPU lock up (double exception)",              // ESP_RST_CPU_LOCKUP
            ];

            document.getElementById("reset-reason").textContent = "Reset Reason: " + resetReasons[resetReason];

            // document.getElementById("reset-reason").textContent = "Reset Reason: " + resetReason;
        })
        .catch(error => {
            console.error("Erro ao obter Informações de Reset:", error);
            document.getElementById("reset-reason").textContent = "Reset Reason: Erro";
            document.getElementById("reset-reason").style.color = "red"; // Destaca visualmente o erro
        });
}

async function fetchUptime() {
    fetch('/uptime')
        .then(response => {
            if (!response.ok) {
                throw new Error("Falha ao obter Informações de Uptime (HTTP " + response.status + ")");
            }
            return response.text();
        })
        .then(uptime => {
            console.log("Informações de Uptime:", uptime);
            const uptimeInMilliseconds = parseInt(uptime);
            const days = Math.floor(uptimeInMilliseconds / (1000 * 60 * 60 * 24));
            const hours = Math.floor((uptimeInMilliseconds % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
            const minutes = Math.floor((uptimeInMilliseconds % (1000 * 60 * 60)) / (1000 * 60));
            const seconds = Math.floor((uptimeInMilliseconds % (1000 * 60)) / 1000);
            const milliseconds = uptimeInMilliseconds % 1000;
            document.getElementById("uptime").textContent = `Uptime: ${days}d ${hours}h ${minutes}m ${seconds}s ${milliseconds}ms`;
        })
        .catch(error => {
            console.error("Erro ao obter Informações de Uptime:", error);
            document.getElementById("uptime").textContent = "Uptime: Erro";
            document.getElementById("uptime").style.color = "red"; // Destaca visualmente o erro
        });
}

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
    await fetchUptime();
    await fetchResetReason();
};