# Web Build Setup

This directory contains the HTML shell template for building the Delphinis engine to WebAssembly.

## GitHub Pages Deployment

The project is set up to automatically deploy to GitHub Pages when changes are pushed to the `main` branch.

### Setup Instructions

1. **Enable GitHub Pages** in your repository settings:
   - Go to Settings > Pages
   - Under "Build and deployment", select "GitHub Actions" as the source

2. **Push to main branch**:
   - The workflow will automatically build and deploy
   - The game will be available at `https://<username>.github.io/<repository>/`

## Local Emscripten Build

To build locally for web:

```bash
# Install Emscripten
# https://emscripten.org/docs/getting_started/downloads.html

# Build
mkdir build-web
cd build-web
emcmake cmake ..
emmake make pong

# Run local server
python3 -m http.server 8000

# Open http://localhost:8000/bin/pong.html
```

## Files

- `shell.html` - HTML template used by Emscripten
- The template includes styled canvas, controls, and loading indicators

## Browser Compatibility

- Modern browsers with WebGL 2.0 support
- Chrome 56+, Firefox 51+, Safari 15+, Edge 79+
