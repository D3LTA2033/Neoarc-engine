// NeoArc Engine - Main JS (script.js)
// Handles all page animations & interactions

// === Configuration ===
const GAMES_JSON_PATH = '/games/games.json';
const TYPING_SPEED = 70; // ms per char for terminal typing
const TYPING_COMMANDS = [
  'neoarc install <game>',
  'neoarc search stardust',
  'neoarc update',
];

// === Utility: Debounce (for performant search) ===
function debounce(fn, ms) {
  let timer;
  return (...args) => {
    clearTimeout(timer);
    timer = setTimeout(() => fn.apply(this, args), ms);
  };
}

// === GAME CARDS: Dynamic Rendering ===
async function fetchGamesData() {
  try {
    const res = await fetch(GAMES_JSON_PATH);
    if (!res.ok) throw new Error('Failed to load games data');
    return await res.json();
  } catch (e) {
    console.error('[NeoArc]', e);
    return [];
  }
}

function renderGames(data, filter = '') {
  const container = document.querySelector('.games-container');
  if (!container) return;

  let filteredGames = data;
  if (filter) {
    const q = filter.trim().toLowerCase();
    filteredGames = data.filter(game =>
      game.name.toLowerCase().includes(q) ||
      (game.genre && game.genre.toLowerCase().includes(q)) ||
      (game.platforms && game.platforms.join(',').toLowerCase().includes(q))
    );
  }

  container.innerHTML = filteredGames.length
    ? filteredGames.map(game => {
      const platforms = Array.isArray(game.platforms) ? game.platforms.join(', ') : '';
      return `
        <div class="game-card fade-up">
          <div class="game-title">${game.name || ''}</div>
          <div class="game-meta">
            <span class="game-genre">${game.genre || ''}</span> ·
            <span class="game-platforms">${platforms}</span>
          </div>
          <div class="game-version">v${game.version || ''}</div>
          <div class="game-install">
            <code>neoarc install ${game.install || game.name?.toLowerCase().replace(/\s/g, '-') || ''}</code>
          </div>
        </div>
      `;
    }).join('')
    : `<div class="no-results">No games found.</div>`;
}

// === ANIMATIONS: Fade Up on Scroll using Intersection Observer ===
function initAnimations() {
  const fadeUps = document.querySelectorAll('.fade-up');
  if (!fadeUps.length) return;

  const observer = new window.IntersectionObserver((entries, obs) => {
    entries.forEach(entry => {
      if (entry.isIntersecting) {
        entry.target.classList.add('is-visible');
        obs.unobserve(entry.target);
      }
    });
  }, { threshold: 0.15 });

  fadeUps.forEach(el => observer.observe(el));
}

// === BUTTONS: Smooth Hover Effects via CSS class toggle (no inline handlers) ===
function initButtonHovers() {
  document.querySelectorAll('.btn').forEach(btn => {
    btn.addEventListener('mouseenter', () => btn.classList.add('btn-hover'));
    btn.addEventListener('mouseleave', () => btn.classList.remove('btn-hover'));
  });
}

// === SEARCH & FILTER: Dynamic Search ===
function initSearch(gamesData) {
  const searchInputs = document.querySelectorAll('[data-games-search]');
  if (!searchInputs.length) return;

  searchInputs.forEach(input => {
    input.addEventListener('input', debounce(e => {
      renderGames(gamesData, e.target.value);
      initAnimations(); // reanimate new elements
    }, 150));
  });
}

// === TERMINAL: Typing Animation in CLI Box ===
function terminalTypingEffect() {
  const terminalBox = document.querySelector('.cli-terminal');
  if (!terminalBox) return;

  let commandIdx = 0;
  let charIdx = 0;
  let currentTimeout;
  let isRunning = true;

  function blinkCursor(active = true) {
    const cursor = terminalBox.querySelector('.cli-cursor');
    if (cursor) cursor.classList.toggle('blink', active);
  }

  function type() {
    if (!isRunning) return;
    const fullCmd = TYPING_COMMANDS[commandIdx];
    terminalBox.innerHTML =
      `<span class="cli-prompt">$</span> ` +
      `<span class="cli-text">${fullCmd.slice(0, charIdx)}</span>` +
      `<span class="cli-cursor blink">|</span>`;

    if (charIdx < fullCmd.length) {
      blinkCursor();
      charIdx++;
      currentTimeout = setTimeout(type, TYPING_SPEED + Math.random() * 80);
    } else {
      blinkCursor(false);
      setTimeout(() => {
        // blink cursor on full command for a short while
        terminalBox.querySelector('.cli-cursor').classList.add('blink');
        setTimeout(() => {
          charIdx = 0;
          commandIdx = (commandIdx + 1) % TYPING_COMMANDS.length;
          type();
        }, 1050);
      }, 600);
    }
  }

  // Start animation
  type();

  // Optionally: add a way to restart effect if needed
  return function stop() {
    isRunning = false;
    clearTimeout(currentTimeout);
    terminalBox.innerHTML = '';
  };
}

// === PAGE INIT: Only runs on DOMContentLoaded ===
document.addEventListener('DOMContentLoaded', async () => {
  // 1. Initialize any present CLI terminal
  terminalTypingEffect();

  // 2. Setup smooth hover effects on all buttons with .btn class
  initButtonHovers();

  // 3. If the page has a .games-container, dynamically load + render games
  const gamesContainer = document.querySelector('.games-container');
  if (gamesContainer) {
    const games = await fetchGamesData();
    renderGames(games, '');
    initAnimations();
    initSearch(games); // Setup live search/filter
  } else {
    // Still animate anything with fade-up
    initAnimations();
  }
});

// === EXPORTS (for future scalability/module support if imported) ===
window.neoarc = {
  renderGames,
  initAnimations,
  initSearch,
  terminalTypingEffect
};