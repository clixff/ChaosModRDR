const fs = require('fs');
const path = require('path');

fs.copyFileSync(path.join('src', 'front', 'index.html'), path.join('dist', 'front', 'index.html'));
fs.copyFileSync(path.join('src', 'front', 'style.css'), path.join('dist', 'front', 'style.css'));
fs.copyFileSync(path.join('src', 'front', 'token.html'), path.join('dist', 'front', 'token.html'));