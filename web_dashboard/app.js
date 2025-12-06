// ============================================
// Global Variables
// ============================================
let sensorChart;
let chartData = {
    labels: [],
    temperature: [],
    humidity: [],
    soilMoisture: [],
    lightLevel: []
};
const MAX_DATA_POINTS = 20;

// ============================================
// DOM Elements
// ============================================
const elements = {
    // Status
    statusIndicator: document.getElementById('statusIndicator'),

    // Controls
    autoModeToggle: document.getElementById('autoModeToggle'),
    pumpBtn: document.getElementById('pumpBtn'),
    ledBtn: document.getElementById('ledBtn'),

    // Sensors
    temperature: document.getElementById('temperature'),
    humidity: document.getElementById('humidity'),
    soilMoisture: document.getElementById('soilMoisture'),
    lightLevel: document.getElementById('lightLevel'),

    // Time stamps
    tempTime: document.getElementById('tempTime'),
    humTime: document.getElementById('humTime'),
    soilTime: document.getElementById('soilTime'),
    lightTime: document.getElementById('lightTime'),

    // Settings
    soilThreshold: document.getElementById('soilThreshold'),
    lightThreshold: document.getElementById('lightThreshold'),
    updateSoilBtn: document.getElementById('updateSoilBtn'),
    updateLightBtn: document.getElementById('updateLightBtn')
};

// ============================================
// Initialize Chart
// ============================================
function initChart() {
    const ctx = document.getElementById('sensorChart').getContext('2d');

    sensorChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: chartData.labels,
            datasets: [
                {
                    label: 'Nhiệt độ (°C)',
                    data: chartData.temperature,
                    borderColor: '#ef4444',
                    backgroundColor: 'rgba(239, 68, 68, 0.1)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Độ ẩm KK (%)',
                    data: chartData.humidity,
                    borderColor: '#3b82f6',
                    backgroundColor: 'rgba(59, 130, 246, 0.1)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Độ ẩm đất (%)',
                    data: chartData.soilMoisture,
                    borderColor: '#10b981',
                    backgroundColor: 'rgba(16, 185, 129, 0.1)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Ánh sáng',
                    data: chartData.lightLevel,
                    borderColor: '#f59e0b',
                    backgroundColor: 'rgba(245, 158, 11, 0.1)',
                    tension: 0.4,
                    fill: true,
                    yAxisID: 'y1'
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: true,
            interaction: {
                mode: 'index',
                intersect: false
            },
            plugins: {
                legend: {
                    labels: {
                        color: '#f1f5f9',
                        font: {
                            family: 'Inter',
                            size: 12
                        }
                    }
                },
                tooltip: {
                    backgroundColor: '#1e293b',
                    titleColor: '#f1f5f9',
                    bodyColor: '#f1f5f9',
                    borderColor: '#334155',
                    borderWidth: 1
                }
            },
            scales: {
                x: {
                    grid: {
                        color: '#334155'
                    },
                    ticks: {
                        color: '#94a3b8'
                    }
                },
                y: {
                    type: 'linear',
                    display: true,
                    position: 'left',
                    grid: {
                        color: '#334155'
                    },
                    ticks: {
                        color: '#94a3b8'
                    }
                },
                y1: {
                    type: 'linear',
                    display: true,
                    position: 'right',
                    grid: {
                        drawOnChartArea: false
                    },
                    ticks: {
                        color: '#94a3b8'
                    }
                }
            }
        }
    });
}

// ============================================
// Update Chart Data
// ============================================
function updateChart(data) {
    const now = new Date();
    const timeLabel = now.toLocaleTimeString('vi-VN', {
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit'
    });

    chartData.labels.push(timeLabel);
    chartData.temperature.push(data.temperature);
    chartData.humidity.push(data.humidity);
    chartData.soilMoisture.push(data.soilMoisture);
    chartData.lightLevel.push(data.lightLevel);

    // Keep only last MAX_DATA_POINTS
    if (chartData.labels.length > MAX_DATA_POINTS) {
        chartData.labels.shift();
        chartData.temperature.shift();
        chartData.humidity.shift();
        chartData.soilMoisture.shift();
        chartData.lightLevel.shift();
    }

    sensorChart.update();
}

// ============================================
// Update Connection Status
// ============================================
function updateConnectionStatus(connected) {
    if (connected) {
        elements.statusIndicator.classList.add('connected');
        elements.statusIndicator.classList.remove('disconnected');
        elements.statusIndicator.querySelector('.status-text').textContent = 'Đã kết nối';
    } else {
        elements.statusIndicator.classList.remove('connected');
        elements.statusIndicator.classList.add('disconnected');
        elements.statusIndicator.querySelector('.status-text').textContent = 'Mất kết nối';
    }
}

// ============================================
// Update Sensor Display
// ============================================
function updateSensorDisplay(data) {
    const now = new Date().toLocaleTimeString('vi-VN');

    elements.temperature.textContent = data.temperature.toFixed(1);
    elements.humidity.textContent = data.humidity.toFixed(1);
    elements.soilMoisture.textContent = data.soilMoisture;
    elements.lightLevel.textContent = data.lightLevel;

    elements.tempTime.textContent = `Cập nhật: ${now}`;
    elements.humTime.textContent = `Cập nhật: ${now}`;
    elements.soilTime.textContent = `Cập nhật: ${now}`;
    elements.lightTime.textContent = `Cập nhật: ${now}`;

    // Update chart
    updateChart(data);
}

// ============================================
// Update Control Buttons
// ============================================
function updateControlButtons(actuators, autoMode) {
    // Update pump button
    if (actuators.pump) {
        elements.pumpBtn.classList.add('active');
        elements.pumpBtn.querySelector('span').textContent = 'BẬT';
    } else {
        elements.pumpBtn.classList.remove('active');
        elements.pumpBtn.querySelector('span').textContent = 'TẮT';
    }

    // Update LED button
    if (actuators.led) {
        elements.ledBtn.classList.add('active');
        elements.ledBtn.querySelector('span').textContent = 'BẬT';
    } else {
        elements.ledBtn.classList.remove('active');
        elements.ledBtn.querySelector('span').textContent = 'TẮT';
    }

    // Enable/disable manual controls based on auto mode
    elements.pumpBtn.disabled = autoMode;
    elements.ledBtn.disabled = autoMode;
}

// ============================================
// Listen to Firebase Data
// ============================================
function listenToFirebase() {
    // Listen to sensor data
    deviceRef.child('sensors').on('value', (snapshot) => {
        const data = snapshot.val();
        if (data) {
            updateSensorDisplay(data);
            updateConnectionStatus(true);
        }
    });

    // Listen to actuator states
    deviceRef.child('actuators').on('value', (snapshot) => {
        const actuators = snapshot.val();
        if (actuators) {
            deviceRef.child('settings/autoMode').once('value', (autoSnapshot) => {
                const autoMode = autoSnapshot.val() !== false;
                updateControlButtons(actuators, autoMode);
            });
        }
    });

    // Listen to settings
    deviceRef.child('settings').on('value', (snapshot) => {
        const settings = snapshot.val();
        if (settings) {
            elements.autoModeToggle.checked = settings.autoMode !== false;
            if (settings.soilMoistureThreshold) {
                elements.soilThreshold.value = settings.soilMoistureThreshold;
            }
            if (settings.lightThreshold) {
                elements.lightThreshold.value = settings.lightThreshold;
            }
        }
    });

    // Monitor connection
    const connectedRef = database.ref('.info/connected');
    connectedRef.on('value', (snapshot) => {
        updateConnectionStatus(snapshot.val() === true);
    });
}

// ============================================
// Control Functions
// ============================================
function toggleAutoMode() {
    const autoMode = elements.autoModeToggle.checked;
    deviceRef.child('settings/autoMode').set(autoMode);

    console.log(`Auto mode: ${autoMode ? 'ON' : 'OFF'}`);
}

function togglePump() {
    const currentState = elements.pumpBtn.classList.contains('active');
    deviceRef.child('actuators/pump').set(!currentState);

    console.log(`Pump: ${!currentState ? 'ON' : 'OFF'}`);
}

function toggleLED() {
    const currentState = elements.ledBtn.classList.contains('active');
    deviceRef.child('actuators/led').set(!currentState);

    console.log(`LED: ${!currentState ? 'ON' : 'OFF'}`);
}

function updateSoilThreshold() {
    const threshold = parseInt(elements.soilThreshold.value);
    if (threshold >= 0 && threshold <= 100) {
        deviceRef.child('settings/soilMoistureThreshold').set(threshold);
        console.log(`Soil threshold updated: ${threshold}%`);

        // Show feedback
        elements.updateSoilBtn.innerHTML = '<i class="fas fa-check"></i> Đã cập nhật!';
        setTimeout(() => {
            elements.updateSoilBtn.innerHTML = '<i class="fas fa-check"></i> Cập nhật';
        }, 2000);
    }
}

function updateLightThreshold() {
    const threshold = parseInt(elements.lightThreshold.value);
    if (threshold >= 0 && threshold <= 1023) {
        deviceRef.child('settings/lightThreshold').set(threshold);
        console.log(`Light threshold updated: ${threshold}`);

        // Show feedback
        elements.updateLightBtn.innerHTML = '<i class="fas fa-check"></i> Đã cập nhật!';
        setTimeout(() => {
            elements.updateLightBtn.innerHTML = '<i class="fas fa-check"></i> Cập nhật';
        }, 2000);
    }
}

// ============================================
// Event Listeners
// ============================================
function setupEventListeners() {
    elements.autoModeToggle.addEventListener('change', toggleAutoMode);
    elements.pumpBtn.addEventListener('click', togglePump);
    elements.ledBtn.addEventListener('click', toggleLED);
    elements.updateSoilBtn.addEventListener('click', updateSoilThreshold);
    elements.updateLightBtn.addEventListener('click', updateLightThreshold);
}

// ============================================
// Initialize App
// ============================================
function initApp() {
    console.log('🚀 Initializing Smart Irrigation Dashboard...');

    // Initialize chart
    initChart();

    // Setup event listeners
    setupEventListeners();

    // Start listening to Firebase
    listenToFirebase();

    console.log('✓ Dashboard ready!');
}

// ============================================
// Start App when DOM is ready
// ============================================
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initApp);
} else {
    initApp();
}

// ============================================
// Service Worker Registration (for PWA)
// ============================================
if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('service-worker.js')
            .then(registration => {
                console.log('✓ Service Worker registered:', registration);
            })
            .catch(error => {
                console.log('✗ Service Worker registration failed:', error);
            });
    });
}
