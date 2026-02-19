/**
 * NeoArc Engine – Main script
 * Navbar scroll, smooth scroll, CLI typing, renderGames, renderLearning, animations
 */

const GAMES_JSON_PATH = '/games/games.json';
const LEARNING_JSON_PATH = '/learning/learning.json';
const TYPING_SPEED = 60;
const TYPING_COMMANDS = [
  'frecli search fps',
  'frecli install voidstrike',
  'frecli run intro-js'
];

function debounce(fn, ms) {
  let t;
  return (...args) => {
    clearTimeout(t);
    t = setTimeout(() => fn.apply(this, args), ms);
  };
}

// ---------- Navbar shadow on scroll ----------
function initNavbarScroll() {
  const navbar = document.getElementById('navbar');
  if (!navbar) return;
  const onScroll = () => {
    if (window.scrollY > 20) navbar.classList.add('navbar--scrolled');
    else navbar.classList.remove('navbar--scrolled');
  };
  window.addEventListener('scroll', debounce(onScroll, 50), { passive: true });
  onScroll();
}

// ---------- Smooth scroll for anchor links ----------
function initSmoothScroll() {
  document.querySelectorAll('a[href^="#"]').forEach(a => {
    const href = a.getAttribute('href');
    if (href === '#') return;
    const target = document.querySelector(href);
    if (target) {
      a.addEventListener('click', (e) => {
        e.preventDefault();
        target.scrollIntoView({ behavior: 'smooth', block: 'start' });
      });
    }
  });
}

// ---------- Fade-up on scroll (Intersection Observer) ----------
function initScrollAnimations() {
  const els = document.querySelectorAll('[data-animate="fade-up"]');
  if (!els.length) return;
  const observer = new IntersectionObserver((entries, obs) => {
    entries.forEach((entry) => {
      if (entry.isIntersecting) {
        entry.target.classList.add('animated');
        obs.unobserve(entry.target);
      }
    });
  }, { threshold: 0.1, rootMargin: '0px 0px -20px 0px' });
  els.forEach((el) => observer.observe(el));
}

// ---------- Footer year ----------
function initFooterYear() {
  const yearEl = document.getElementById('year');
  if (yearEl) yearEl.textContent = new Date().getFullYear();
}

// ---------- CLI terminal typing (landing page only) ----------
function initCLITyping() {
  if (document.body.classList.contains('page--download')) return;
  const body = document.querySelector('.cli-terminal__body');
  if (!body || body.classList.contains('cli-typing-initialized')) return;
  body.classList.add('cli-typing-initialized');
  let cmdIdx = 0;
  let charIdx = 0;
  let timeoutId;
  let running = true;

  function type() {
    if (!running) return;
    const full = TYPING_COMMANDS[cmdIdx];
    const prompt = '<span class="cli-prompt">$</span> ';
    const text = full.slice(0, charIdx);
    const cursor = '<span class="cli-cursor">|</span>';
    body.innerHTML = prompt + escapeHtml(text) + cursor;

    if (charIdx < full.length) {
      charIdx++;
      timeoutId = setTimeout(type, TYPING_SPEED + Math.random() * 30);
    } else {
      charIdx = 0;
      cmdIdx = (cmdIdx + 1) % TYPING_COMMANDS.length;
      timeoutId = setTimeout(type, 1200);
    }
  }
  function escapeHtml(s) {
    const div = document.createElement('div');
    div.textContent = s;
    return div.innerHTML;
  }
  type();
  window._neoarcStopCLI = () => {
    running = false;
    clearTimeout(timeoutId);
  };
}

// ---------- Fetch games ----------
async function fetchGames() {
  try {
    const res = await fetch(GAMES_JSON_PATH);
    if (!res.ok) throw new Error('Failed to load games');
    const data = await res.json();
    return Array.isArray(data) ? data : (data.games || []);
  } catch (e) {
    console.error('[NeoArc]', e);
    return [];
  }
}

// ---------- Fetch learning ----------
async function fetchLearning() {
  try {
    const res = await fetch(LEARNING_JSON_PATH);
    if (!res.ok) throw new Error('Failed to load learning');
    const data = await res.json();
    return Array.isArray(data) ? data : [];
  } catch (e) {
    console.error('[NeoArc]', e);
    return [];
  }
}

// ---------- Render games into container ----------
function renderGames(games, filter, containerSelector) {
  const container = containerSelector
    ? document.querySelector(containerSelector)
    : (document.querySelector('.games-container') || document.getElementById('games-list'));
  if (!container) return;
  const q = (filter || '').trim().toLowerCase();
  const list = q
    ? games.filter(
        (g) =>
          (g.name && g.name.toLowerCase().includes(q)) ||
          (g.genre && g.genre.toLowerCase().includes(q)) ||
          (g.id && g.id.toLowerCase().includes(q)) ||
          (Array.isArray(g.platforms) && g.platforms.some((p) => String(p).toLowerCase().includes(q)))
      )
    : games;

  const emptyEl = document.getElementById('games-empty') || container.nextElementSibling;
  if (list.length === 0) {
    container.innerHTML = '';
    if (emptyEl && emptyEl.classList && (emptyEl.classList.contains('no-results') || emptyEl.id === 'games-empty')) {
      emptyEl.style.display = '';
    }
    return;
  }
  if (emptyEl && emptyEl.id === 'games-empty') emptyEl.style.display = 'none';

  const platformSpans = (g) => (Array.isArray(g.platforms) ? g.platforms : []).map((p) => `<span class="games-card__platform">${escapeHtml(String(p))}</span>`).join('');
  container.innerHTML = list
    .map(
      (game, i) => `
    <article class="games-card" data-animate="fade-up" data-animate-delay="${i * 80}" tabindex="0">
      <header class="games-card__header">
        <h3 class="games-card__title">${escapeHtml(game.name || '')}</h3>
        <span class="games-card__genre">${escapeHtml(game.genre || '')}</span>
      </header>
      <div class="games-card__meta">${platformSpans(game)}</div>
      <div class="games-card__version">Version <span class="games-card__vernum">${escapeHtml(game.version || '')}</span></div>
      <div class="games-card__install">
        <code class="games-card__cmd">frecli install ${escapeHtml(game.id || '')}</code>
        <button type="button" class="btn btn--primary btn--install" data-game="${escapeHtml(game.id || '')}">Copy</button>
      </div>
    </article>
  `
    )
    .join('');

  initScrollAnimations();
  container.querySelectorAll('.btn--install').forEach((btn) => {
    btn.addEventListener('click', () => {
      const id = btn.getAttribute('data-game');
      const cmd = `frecli install ${id}`;
      navigator.clipboard && navigator.clipboard.writeText(cmd).then(() => {
        btn.textContent = 'Copied!';
        setTimeout(() => { btn.textContent = 'Copy'; }, 1200);
      });
    });
  });
}

// ---------- Render learning entries ----------
function renderLearning(entries, filter, containerSelector) {
  const container = containerSelector
    ? document.querySelector(containerSelector)
    : (document.querySelector('.learning-container') || document.getElementById('learning-list'));
  if (!container) return;
  const q = (filter || '').trim().toLowerCase();
  const list = q
    ? entries.filter(
        (e) =>
          (e.title && e.title.toLowerCase().includes(q)) ||
          (e.type && e.type.toLowerCase().includes(q)) ||
          (e.difficulty && e.difficulty.toLowerCase().includes(q)) ||
          (e.description && e.description.toLowerCase().includes(q))
      )
    : entries;

  const emptyEl = document.getElementById('learning-empty');
  if (list.length === 0) {
    container.innerHTML = '<div class="no-results learning-empty">No tutorials or exercises found.</div>';
    return;
  }

  container.innerHTML = list
    .map(
      (entry, i) => `
    <article class="learning-card" data-animate="fade-up" data-animate-delay="${i * 80}" tabindex="0">
      <h3 class="learning-card__title">${escapeHtml(entry.title || '')}</h3>
      <span class="learning-card__type">${escapeHtml(entry.type || '')}</span>
      <span class="learning-card__difficulty">${escapeHtml(entry.difficulty || '')}</span>
      <p class="learning-card__desc">${escapeHtml(entry.description || '')}</p>
      <div class="learning-card__command">
        <code class="learning-card__cmd">${escapeHtml(entry.command || '')}</code>
      </div>
    </article>
  `
    )
    .join('');

  initScrollAnimations();
}

function escapeHtml(s) {
  if (s == null) return '';
  const div = document.createElement('div');
  div.textContent = s;
  return div.innerHTML;
}

// ---------- Search page: combined games + learning ----------
function renderSearchResults(games, learning, query, containerSelector) {
  const container = document.querySelector(containerSelector) || document.getElementById('search-results-list');
  const emptyEl = document.getElementById('search-empty-message');
  if (!container) return;
  const q = (query || '').trim().toLowerCase();
  const filteredGames = q
    ? games.filter(
        (g) =>
          (g.name && g.name.toLowerCase().includes(q)) ||
          (g.genre && g.genre.toLowerCase().includes(q)) ||
          (g.id && g.id.toLowerCase().includes(q)) ||
          (Array.isArray(g.platforms) && g.platforms.some((p) => String(p).toLowerCase().includes(q)))
      )
    : games;
  const filteredLearning = q
    ? learning.filter(
        (e) =>
          (e.title && e.title.toLowerCase().includes(q)) ||
          (e.type && e.type.toLowerCase().includes(q)) ||
          (e.difficulty && e.difficulty.toLowerCase().includes(q)) ||
          (e.description && e.description.toLowerCase().includes(q))
      )
    : learning;

  const items = [];
  filteredGames.forEach((g, i) => {
    const platforms = Array.isArray(g.platforms) ? g.platforms.join(', ') : '';
    items.push({
      type: 'game',
      title: g.name,
      meta: `Genre: ${g.genre || '—'} · Platforms: ${platforms} · v${g.version || '—'}`,
      cmd: `frecli install ${g.id || ''}`,
      delay: i * 60
    });
  });
  filteredLearning.forEach((e, i) => {
    items.push({
      type: 'learning',
      title: e.title,
      meta: `Type: ${e.type || '—'} · Difficulty: ${e.difficulty || '—'}`,
      cmd: e.command || '',
      delay: (filteredGames.length + i) * 60
    });
  });

  if (items.length === 0) {
    container.innerHTML = '';
    if (emptyEl) emptyEl.style.display = '';
    return;
  }
  if (emptyEl) emptyEl.style.display = 'none';

  container.innerHTML = items
    .map(
      (item) => `
    <article class="search-result-card" data-animate="fade-up" data-animate-delay="${item.delay}" tabindex="0">
      <h2 class="search-result-card__title">${escapeHtml(item.title)}</h2>
      <p class="search-result-card__meta">${escapeHtml(item.meta)}</p>
      <code class="search-result-card__cmd">${escapeHtml(item.cmd)}</code>
    </article>
  `
    )
    .join('');

  initScrollAnimations();
}

// ---------- Init ----------
document.addEventListener('DOMContentLoaded', async () => {
  initNavbarScroll();
  initSmoothScroll();
  initScrollAnimations();
  initFooterYear();
  initCLITyping();
  initFAQAccordion();
  initContactForm();

  // Games page
  const gamesList = document.getElementById('games-list');
  const gamesSearchInput = document.getElementById('games-search-input');
  if (gamesList) {
    const games = await fetchGames();
    renderGames(games, '', '#games-list');
    if (gamesSearchInput) {
      gamesSearchInput.addEventListener(
        'input',
        debounce((e) => {
          renderGames(games, e.target.value, '#games-list');
        }, 150)
      );
    }
  }

  // Learning page
  const learningList = document.getElementById('learning-list');
  const learningSearchInput = document.getElementById('learning-search-input');
  if (learningList) {
    const learning = await fetchLearning();
    renderLearning(learning, '', '#learning-list');
    if (learningSearchInput) {
      learningSearchInput.addEventListener(
        'input',
        debounce((e) => {
          renderLearning(learning, e.target.value, '#learning-list');
        }, 150)
      );
    }
  }

  // Search page (games + learning)
  const searchResultsList = document.getElementById('search-results-list');
  const searchInput = document.getElementById('game-search-input') || document.getElementById('search-input');
  if (searchResultsList) {
    const [games, learning] = await Promise.all([fetchGames(), fetchLearning()]);
    renderSearchResults(games, learning, '', '#search-results-list');
    if (searchInput) {
      searchInput.addEventListener(
        'input',
        debounce((e) => {
          renderSearchResults(games, learning, e.target.value, '#search-results-list');
        }, 120)
      );
    }
  }

  // Wiki page: filter tutorial cards
  const wikiSearchInput = document.getElementById('wiki-search-input');
  const wikiTutorialsList = document.getElementById('wiki-tutorials-list');
  const wikiEmpty = document.getElementById('wiki-empty');
  if (wikiSearchInput && wikiTutorialsList) {
    const wikiCards = Array.from(wikiTutorialsList.querySelectorAll('.tutorial-card'));
    wikiSearchInput.addEventListener(
      'input',
      debounce(() => {
        const q = wikiSearchInput.value.toLowerCase().trim();
        let visible = 0;
        wikiCards.forEach((card) => {
          const show = !q || card.textContent.toLowerCase().includes(q);
          card.style.display = show ? '' : 'none';
          if (show) visible++;
        });
        if (wikiEmpty) wikiEmpty.style.display = visible === 0 ? '' : 'none';
      }, 150)
    );
  }
});

// Export for reuse
window.neoarc = {
  fetchGames,
  fetchLearning,
  renderGames,
  renderLearning,
  renderSearchResults,
  initScrollAnimations,
  debounce
};
