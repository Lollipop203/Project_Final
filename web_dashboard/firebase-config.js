// ============================================
// Firebase Configuration
// ============================================
// Thông tin từ Firebase project: smart-irrigation-system-1cf35

const firebaseConfig = {
    apiKey: "AIzaSyDYTPEIx_e6--JmoIJdBg7xzLKidmKa0nI",
    authDomain: "smart-irrigation-system-1cf35.firebaseapp.com",
    databaseURL: "https://smart-irrigation-system-1cf35-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "smart-irrigation-system-1cf35",
    storageBucket: "smart-irrigation-system-1cf35.firebasestorage.app",
    messagingSenderId: "845023170000",
    appId: "1:845023170000:web:89188ab16b36f90a683dd5",
    measurementId: "G-5F1VJ9PP9V"
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);

// Get database reference
const database = firebase.database();
const deviceRef = database.ref('devices/device_001');

console.log('✓ Firebase initialized');
console.log('✓ Project:', firebaseConfig.projectId);
console.log('✓ Database:', firebaseConfig.databaseURL);
