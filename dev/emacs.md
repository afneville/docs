---
title: Emacs Initialisation
author: Alexander Neville
date: 2024-02-24
description:
  A collection of Emacs Lisp snippets for configuring GNU Emacs.
image-meta: emacs-logo.png
image-meta-credit: https://commons.wikimedia.org/wiki/File:Emacs-logo.svg
---

Emacs is a highly extensible _text-oriented user interface_, with
capabilities extending far beyond what most other text editors offer.
This blog post is a complete Emacs _literate_ configuration file and can
be used to initialise the program with my preferences. I have also drawn
some Emacs Lisp code from another stand-alone export script. These two
sources have their own repositories:

- [Emacs Initialisation](https://github.com/afneville/emacs-config)
- [Emacs Org Publish Script](https://github.com/afneville/org-publish-script)

Emacs additionally provides an interpreter for Emacs Lisp, the
[functional programming](./functional.md) language in which much of its
codebase is written. Emacs is extensible via code written in this
language, examples of which are given here.

# Initialisation

So that this file can be rendered nicely on GitHub or here on my
website, it is called `README.org`, _Org_ being a markup language (you
are currently reading a version of this file that has been transformed
to Markdown, edited and then transformed to HTML). Emacs will not expect
its configuration to be found in such file; it is the contents of
`init.el` that are loaded by Emacs on startup. There are two ways the
contents of a literate configuration document could be extracted to a
plain text file:

1.  Load the file in Emacs and export the contents of the source blocks
    to a clean file.
2.  Extract the contents of the source block via an Emacs Lisp script or
    even the `init.el` file itself.

The second of these options is preferable as a new installation can be
bootstrapped without an existing Emacs environment. A minimal `init.el`
capable of loading a literate configuration might look like:

```lisp
(setq custom-file (locate-user-emacs-file "custom_vars.el"))
(load custom-file 'noerror 'nomessage)
(require 'package)
(setq package-archives '(("melpa" . "https://melpa.org/packages/")
                         ("org" . "https://orgmode.org/elpa/")
                         ("elpa" . "https://elpa.gnu.org/packages/")))
(package-initialize)
(unless package-archive-contents
  (package-refresh-contents))
(unless (package-installed-p 'use-package)
  (package-install 'use-package))
(require 'use-package)
(setq use-package-always-ensure t)
(require 'org)
(org-babel-load-file
  (expand-file-name "README.org"
                    user-emacs-directory))
```

Unfortunately, this approach requires that Org is loaded before the
contents of the custom configuration. This means that package repository
initialisation must take place first to ensure that a single Org version
is used, not loaded partially once and then again in the literate
configuration. In some cases, users have been able to export the
contents of a literate configuration using only primitive Emacs Lisp
functions, however I was understandably hesitant to work out the
required regular expressions. Here is a screenshot of the finished
configuration displaying an _Org_ buffer:

![Screenshot](/res/emacs_screenshot.png){#fig:screenshot}

## Configuration Options

The remainder of the initialisation can be performed from the literate
file itself. The first step is calling some functions to disable some
unhelpful GUI components.

```lisp
(menu-bar-mode -1) (tool-bar-mode -1)
(scroll-bar-mode -1)
(fringe-mode '(0 . 0))
(global-auto-revert-mode 1)
(global-set-key (kbd "<escape>") 'keyboard-escape-quit)
(global-set-key (kbd "M-u") 'universal-argument)
```

Followed by setting the values of some variables.

```lisp
(setq-default indent-tabs-mode nil)
(setq-default c-basic-offset 4)
(setq inhibit-startup-echo-area-message "alex"
  inhibit-startup-message t
  initial-scratch-message "; Hello!"
  tab-width 4
  use-dialog-box nil
  scroll-conservatively 1000
  backup-inhibited t
  auto-save-default nil
  global-auto-revert-non-file-buffers t
  x-select-enable-clipboard nil
  user-full-name "Alexander Neville")
```

The default font is given an absolute height of `100`. The height of
other fonts is specified relative to the height of the default font. In
this case, `:height 1.0` can be omitted.

```lisp
(set-face-attribute 'default nil :family "VictorMono Nerd Font" :height 110)
(set-face-attribute 'variable-pitch nil :family "Noto Sans")
(set-face-attribute 'fixed-pitch nil :family "VictorMono Nerd Font")
```

## Themes

The _modus themes_ are very feature-rich and roll a lot of
initialisation into one package.

```lisp
(use-package modus-themes
  :config
  (setq modus-themes-mode-line '(accented)
        modus-themes-region '(accented bg-only)
        modus-themes-completions '(opinionated)
        modus-themes-bold-constructs t
        modus-themes-italic-constructs t
        modus-themes-paren-match '(bold underline)
        modus-themes-syntax '(faint)
        modus-themes-subtle-line-numbers t
        modus-themes-org-blocks nil
        modus-themes-common-palette-overrides modus-themes-preset-overrides-faint
        modus-themes-common-palette-overrides
        '((fg-heading-1 blue)
          (fg-heading-2 cyan)
          (fg-heading-3 green)
          )
        )
  (load-theme 'modus-operandi t)
  )
```

The set of _doom emacs_ themes is also quite nice. I only ever load one
of _modus_ or _doom_.

```lisp
(use-package doom-themes
  :ensure t
  :config
  (setq doom-themes-enable-bold t
        doom-themes-enable-italic t)
  (doom-themes-org-config)
  (load-theme 'doom-one t))
```

The `all-the-icons` package is required by doom modeline and some other
packages.

```lisp
(use-package all-the-icons
  :ensure t)
```

Optionally include the doom modeline.

```lisp
(use-package doom-modeline
  :ensure t
  :hook (after-init . doom-modeline-mode)
  :config
  (setq doom-modeline-height 50)
  (setq doom-modeline-bar-width 0))
```

_Base16_ is another good theme project for Emacs and other applications.
This package can be loaded in place of one of the other theme packages.

```lisp
(use-package base16-theme
  :ensure t
  :demand
  :init
  (setq base16-theme-distinct-fringe-background nil)
  (setq base16-theme-highlight-mode-line 'contrast)
  (setq base16-theme-256-color-source 'colors)
  :config
  (load-theme 'base16-gruvbox-material-dark-hard t)
                                        ; Set the cursor color based on the evil state
  (defvar myconfig/base16-colors base16-gruvbox-material-dark-hard-theme-colors)
  (setq evil-emacs-state-cursor   `(,(plist-get myconfig/base16-colors :base0D) box)
        evil-insert-state-cursor  `(,(plist-get myconfig/base16-colors :base0D) bar)
        evil-motion-state-cursor  `(,(plist-get myconfig/base16-colors :base0E) box)
        evil-normal-state-cursor  `(,(plist-get myconfig/base16-colors :base0B) box)
        evil-replace-state-cursor `(,(plist-get myconfig/base16-colors :base08) bar)
        evil-visual-state-cursor  `(,(plist-get myconfig/base16-colors :base09) box)))
```

The same is true of _autothemer_.

```lisp
(use-package autothemer
  :ensure t
  :config
  (load-theme 'notebook-light t))
```

## Navigation & Bindings

Improve the default `M-x` behaviour with _fuzzy finding_. _Ivy_ also
includes _swiper_, which is a great way to search a file that the buffer
is visiting.

```lisp
(use-package ivy
  :diminish
  :bind (("C-s" . swiper)
         :map ivy-minibuffer-map
         ("TAB" . ivy-alt-done)
         ("C-l" . ivy-alt-done)
         ("C-j" . ivy-next-line)
         ("C-k" . ivy-previous-line)
         :map ivy-switch-buffer-map
         ("C-k" . ivy-previous-line)
         ("C-l" . ivy-done)
         ("C-d" . ivy-switch-buffer-kill)
         :map ivy-reverse-i-search-map
         ("C-k" . ivy-previous-line)
         ("C-d" . ivy-reverse-i-search-kill))
  :config
  (setq ivy-initial-inputs-alist nil)
  (ivy-mode 1))

(use-package ivy-rich
  :init
  (ivy-rich-mode 1))

(use-package counsel
  :bind (("M-x" . counsel-M-x)
         ("C-x b" . counsel-ibuffer)
         ("C-x C-f" . counsel-find-file)
         :map minibuffer-local-map
         ("C-r" . 'counsel-minibuffer-history)))
```

A reasonably self-explanatory package to provide a nice splash screen
and display an encouraging message.

```lisp
(use-package dashboard
  :ensure t
  :config
  (setq initial-buffer-choice (lambda () (get-buffer-create "*dashboard*"))
        dashboard-banner-logo-title "GNU Emacs"
        dashboard-center-content t
        dashboard-items '((recents  . 5)
                          (projects . 5)
                          )
        dashboard-set-heading-icons t
        dashboard-set-file-icons t
        dashboard-footer-messages '("Do something useful!")
        dashboard-footer-icon (all-the-icons-octicon "dashboard"
                                                     :height 1.1
                                                     :v-adjust -0.05
                                                     :face 'font-lock-keyword-face)
        dashboard-set-init-info t
        )
  (dashboard-setup-startup-hook))
```

I use Vi/Vim keys in Emacs. It would probably be too difficult to use
Emacs keys and Vi keys in different applications simultaneously. I also
use the Vi keys in _tmux_, _zsh_ and my window manager, so Emacs would
be the exception.

```lisp
(use-package evil
  :init
  (setq evil-want-integration t
        evil-want-keybinding nil
        evil-want-C-u-scroll t
        evil-want-C-i-jump nil
        evil-respect-visual-line-mode t
        evil-insert-state-cursor nil)
  :config
  (evil-mode 1)
  (define-key evil-insert-state-map (kbd "C-g") 'evil-normal-state)
  (define-key evil-insert-state-map
    (kbd "C-h") 'evil-delete-backward-char-and-join)
  (define-key evil-normal-state-map (kbd "<return>") nil)
  (evil-global-set-key 'motion "j" 'evil-next-visual-line)
  (evil-global-set-key 'motion "k" 'evil-previous-visual-line)
  (evil-global-set-key 'motion "0" 'evil-beginning-of-visual-line)
  (evil-global-set-key 'motion "$" 'evil-end-of-visual-line)
  (evil-global-set-key 'motion "^" 'evil-first-non-blank-of-visual-line)
  (evil-set-initial-state 'messages-buffer-mode 'normal)
  (evil-set-initial-state 'dashboard-mode 'normal))

(with-eval-after-load 'evil-maps
  (define-key evil-motion-state-map (kbd "RET") nil))

(use-package evil-collection
  :after evil
  :config
  (evil-collection-init))
```

The _Which-key_ package will show you a list of keybindings prefixed by
the current combination after a short delay. This restores some of the
discoverability lost by disabling the UI.

```lisp
(use-package general
  :config
  (general-evil-setup t)
  (general-create-definer myconfig/leader-keys
    :prefix "SPC"
    :global-prefix "C-c"))

(myconfig/leader-keys 'normal 'override
  "bb" 'counsel-switch-buffer
  "ff" 'counsel-find-file
  "ss" 'swiper
  "d" 'dired
  "y" 'clipboard-kill-ring-save
  "e" 'eval-buffer
  "x" 'counsel-M-x
  "i" 'ibuffer
  "c" 'evil-delete-buffer
  "t" 'counsel-load-theme
  "le" '(lambda () (interactive) (find-file "~/.config/emacs/README.org"))
  "or" 'org-mode-restart
  "op" 'org-latex-preview
  "otl" 'org-toggle-link-display
  "oti" 'org-toggle-inline-images
  "otb" 'org-toggle-blocks
  "obt" 'org-toggle-blocks
  "oby" 'org-copy-src-block
  "obY" 'org-clipboard-copy-src-block
  "pp" 'projectile-switch-project
  )

(use-package which-key
  :init (which-key-mode)
  :diminish which-key-mode
  :config
  (setq which-key-idle-delay 0.01))
```

Define some interactive functions for those bindings.

```lisp
(defun org-copy-src-block ()
  (interactive)
  (org-edit-src-code)
  (beginning-of-buffer)
  (mark-end-of-buffer)
  (kill-ring-save (point) (mark))
  (org-edit-src-abort)
  (message "Copied to register!"))

(defun org-clipboard-copy-src-block ()
  (interactive)
  (org-edit-src-code)
  (beginning-of-buffer)
  (mark-end-of-buffer)
  (clipboard-kill-ring-save (point) (mark))
  (org-edit-src-abort)
  (message "Copied to clipboard!"))

(defvar org-blocks-hidden nil)
(defun org-toggle-blocks ()
  (interactive)
  (if org-blocks-hidden
      (org-show-block-all)
    (org-hide-block-all))
  (setq-local org-blocks-hidden (not org-blocks-hidden)))
```

## Programming

This is a very quick programming setup as I won't be using Emacs for
programming very often, preferring Neovim. Enable line highlighting and
line numbers.

```lisp
(setq display-line-numbers-type 'relative)
(add-hook 'prog-mode-hook #'display-line-numbers-mode)
(add-hook 'prog-mode-hook #'hl-line-mode)
```

Optional tree-sitter support.

```lisp
(use-package tree-sitter
  :config
  (global-tree-sitter-mode))
(use-package tree-sitter-langs)
```

Company is a completion front-end.

```lisp
(use-package company
  :config
  (setq company-minimum-prefix-length 1
        company-idle-delay 0
        company-tooltip-limit 6
        company-tempo-expand t
        company-backends '((company-capf company-dabbrev-code company-files company-yasnippet)))
  )
(defun my-company-org-mode-hook ()
  (setq-local company-backends
              '((company-capf company-yasnippet company-dabbrev company-ispell company-tempo :separate)
                )))
(setq ispell-alternate-dictionary "/home/alex/.config/emacs/completion-words.txt")

(with-eval-after-load 'company
  (define-key company-active-map (kbd "TAB") nil))

(add-hook 'org-mode-hook #'my-company-org-mode-hook)
(add-hook 'org-mode-hook #'company-mode)
(add-hook 'text-mode-hook #'company-mode)
(add-hook 'prog-mode-hook #'company-mode)

(use-package company-box
  :hook (company-mode . company-box-mode))
```

Language server support via `lsp-mode`.

```lisp
(use-package lsp-mode
  :init
  (setq lsp-keymap-prefix "C-c l")
  :commands lsp)

(use-package smartparens
  :config
  (require 'smartparens-config)
  (add-hook 'prog-mode-hook #'smartparens-mode))
```

I haven't relied on snippets too heavily in the past, but I am gradually
getting used to the idea. As it is so easy to set up in Emacs, I will
include some snippets.

```lisp
(use-package yasnippet
  :config
  (setq yas-snippet-dirs '("~/.config/emacs/snippets"))
  (add-hook 'prog-mode-hook #'yas-minor-mode)
  (add-hook 'prog-mode-hook #'yas-minor-mode))
  (define-key yas-minor-mode-map (kbd "<tab>") nil)
  (define-key yas-minor-mode-map (kbd "TAB") nil)
  (define-key yas-minor-mode-map (kbd "C-c y") #'yas-expand)
  (yas-global-mode 1))
(use-package yasnippet-snippets)
```

_Projectile_ is a package for managing projects.

```lisp
(use-package projectile
  :config
  (projectile-mode)
  (setq projectile-project-search-path '("~/vcon/")
        projectile-auto-discover t
        projectile-switch-project-action #'projectile-dired)
  )
```

## Org Mode

_Org_ and the _Org major mode_ are greats tool for authoring. Another
strength of Org mode is its handling of source blocks, with which you
can write code notebooks and do literate programming. This document acts
as a literate configuration file for Emacs, for example.

Define a function to evaluate when Org mode is loaded, this is easier
than many `(add-hook 'org-mode-hook ...)` expressions.

```lisp
(defun org-mode-setup ()
  (org-indent-mode t)
  (visual-line-mode nil)
  (setq org-hide-emphasis-markers t
        org-pretty-entities t
        org-return-follows-link t
        org-list-allow-alphabetical t
        org-hide-leading-stars nil
        org-catch-invisible-edits 'smart
        org-use-sub-superscripts '{}
        org-export-with-sub-superscripts '{}
        org-src-preserve-indentation nil
        org-edit-src-content-indentation 2
        org-format-latex-options (plist-put org-format-latex-options :scale 3.0)
        org-src-fontify-natively t
        org-image-actual-width (list 600)
        org-ellipsis " ..."
        org-link-frame-setup
        '((vm . vm-visit-folder-other-frame)
          (vm-imap . vm-visit-imap-folder-other-frame)
          (gnus . org-gnus-no-new-news)
          (file . find-file)
          (wl . wl-other-frame))
        ))
```

Call the setup function when Org mode is loaded.

```lisp
(require 'org-indent)
(require 'org-src)
(setq
  org-startup-folded nil
  org-startup-with-latex-preview nil
  org-startup-with-inline-images nil)
(add-hook 'org-mode-hook 'org-mode-setup)
```

Structure templates make inserting source blocks much easier.

```lisp
(require 'org-tempo)
(add-to-list 'org-structure-template-alist '("sh" . "src shell"))
(add-to-list 'org-structure-template-alist '("el" . "src emacs-lisp"))
(add-to-list 'org-structure-template-alist '("py" . "src python"))
(add-to-list 'org-structure-template-alist '("txt" . "src text"))
```

Org babel is used for literate programming, including handling this very
file.

```lisp
(org-babel-do-load-languages
  'org-babel-load-languages
  '((emacs-lisp . t)
    (python . t)))

(setq org-confirm-babel-evaluate nil)
```

To use `varaible-pitch-mode`, some faces need to be set to mono-space,
such as code blocks and tables.

```lisp
(set-face-attribute 'org-block-begin-line nil :inherit '(shadow fixed-pitch))
(set-face-attribute 'org-block-end-line nil :inherit '(shadow fixed-pitch))
(set-face-attribute 'org-block nil :inherit 'fixed-pitch)
(set-face-attribute 'org-code nil :inherit 'fixed-pitch)
(set-face-attribute 'org-indent nil :inherit '(org-hide fixed-pitch))
(set-face-attribute 'org-document-info-keyword nil :inherit '(shadow fixed-pitch))
(set-face-attribute 'org-verbatim nil :inherit '(shadow fixed-pitch))
(set-face-attribute 'org-special-keyword nil :inherit '(font-lock-comment-face fixed-pitch))
(set-face-attribute 'org-meta-line nil :inherit '(font-lock-comment-face fixed-pitch))
(set-face-attribute 'org-checkbox nil :inherit 'fixed-pitch)
```

The _mixed-pitch_ package does many of the same things.

```lisp
(use-package mixed-pitch
  :hook
  (org-mode . mixed-pitch-mode))
```

Change the height of the face used to display Org mode headings and the
document title.

```lisp
(defun org-heading-face-setup ()
  (dolist (face '((org-level-1 . 1.5)
                  (org-level-2 . 1.4)
                  (org-level-3 . 1.3)
                  (org-level-4 . 1.2)
                  (org-level-5 . 1.1)
                  (org-level-6 . 1.1)
                  (org-level-7 . 1.1)
                  (org-level-8 . 1.1)))
    (set-face-attribute (car face) nil :family
                        (face-attribute 'default :family)
                        :weight 'bold :height (cdr face)))
  )

(add-hook 'org-mode-hook 'org-heading-face-setup)

(defun org-title-face-setup ()
  (set-face-attribute 'org-document-title nil :family (face-attribute 'default :family) :weight 'bold :height 1.6)
  )

(add-hook 'org-mode-hook 'org-title-face-setup)
```

Replace bullet points (lines with leading dashes) with Unicode
characters.

```lisp
(font-lock-add-keywords 'org-mode
                        '(("^ *\\([-]\\) "
                           (0 (prog1 () (compose-region (match-beginning 1) (match-end 1) "•"))))))
```

Ensure LaTeX blocks are highlighted, but keep the default text
background.

```lisp
(setq org-highlight-latex-and-related '(latex native entities))
(add-to-list 'org-src-block-faces '("latex" (:inherit default :extend t)))
```

I am now experimenting with some additional org features, besides those
used for authoring. I am modifying the sequence(s) of `TODO` keywords
and adding a prefix letter to each, for easy navigation. Creating and
changing the state of a `TODO` item records a timestamp and switching to
the cancelled state additionally requires a comment. This information is
placed inside a drawer.

```lisp
(setq org-todo-keywords
      '((sequence "TODO(t!)" "ACTIVE(a!)" "|" "DONE(d!)" "CANCELED(c@)")
        (sequence "QUESTION(q!)"  "|" "SOLVED(s@)"))
      org-log-into-drawer t)
```

Optionally use a package like _org-modern_ to style the buffer. I am not
using it currently.

```lisp
(use-package org-modern
  :hook (org-mode . org-modern-mode))
```

Most of the time I choose to hide a few org syntax elements. The
_org-bullets_ package replaces the asterisks before each heading with
Unicode symbols. _org-appear_ shows emphasis markers when the cursor
hovers over emphasised text.

```lisp
(use-package org-bullets
  :after org
  :hook (org-mode . org-bullets-mode)
  :custom
  (org-bullets-bullet-list '("●" "○" "●" "○" "●" "○" "●")))
```

```lisp
(use-package org-appear
  :hook (org-mode . org-appear-mode)
  :config
  (setq org-appear-autoemphasis t
        org-appear-autolinks t)
  (run-at-time nil nil #'org-appear--set-elements))
```

For writing text, `visual-line-mode` is enabled. The
_visual-fill-column_ package can set the point at which text wraps and
centre text in the buffer, which is very helpful.

```lisp
(use-package visual-fill-column
  :config
  (setq-default visual-fill-column-center-text t)
  (setq-default visual-fill-column-width 100))
(add-hook 'visual-line-mode-hook #'visual-fill-column-mode)
(advice-add 'text-scale-adjust :after #'visual-fill-column-adjust)
```

A couple of packages to help with editing LaTeX in Org mode. Some
snippets have also been loaded.

```lisp
(use-package cdlatex
  :config
  (setq cdlatex-simplify-sub-super-scripts nil)
  (add-hook 'org-mode-hook #'turn-on-org-cdlatex))
(use-package org-fragtog
  :config
  (add-hook 'org-mode-hook 'org-fragtog-mode))
```

Spell checking is an essential feature for writing text. _Ispell_
ignores words in my private dictionary and code inside source blocks.
_Flycheck_ highlights possible errors in the buffer.

```lisp
(setq
  ispell-program-name "aspell"
  ispell-dictionary "en_GB-ise"
  ispell-local-dictionary "en_GB-ise"
  ispell-personal-dictionary "~/.config/emacs/british-ise.pws"
  )
(defun my_config/org-ispell ()
  "Configure `ispell-skip-region-alist' for `org-mode'."
  (make-local-variable 'ispell-skip-region-alist)
  (add-to-list 'ispell-skip-region-alist '(org-property-drawer-re))
  (add-to-list 'ispell-skip-region-alist '("~" "~"))
  (add-to-list 'ispell-skip-region-alist '("=" "="))
  (add-to-list 'ispell-skip-region-alist '("^#\\+BEGIN_SRC" . "^#\\+END_SRC"))
  (add-to-list 'ispell-skip-region-alist '("^#\\+begin_src" . "^#\\+end_src")))
(add-hook 'org-mode-hook #'my_config/org-ispell)
(dolist (hook '(org-mode-hook))
  (add-hook hook (lambda () (flyspell-mode 1))))
(add-hook 'flyspell-mode-hook 'flyspell-buffer)
```

# Org Export & Publish

Emacs org-mode has its own export and publishing system which is highly
configurable and useful for exporting many documents at once. The
relevant sections of the Org Manual:

- [Org Export](https://orgmode.org/manual/Exporting.html)
- [Org Publish](https://orgmode.org/manual/Publishing.html)

Very simply, the export function can be used to convert a single org
document to some other format and the publish utility can convert many
files to some format by using an exporter.

## Setup

Require the exporters for HTML and LaTeX.

```lisp
(require 'ox-html)
(require 'ox-latex)
(require 'ox-publish)
```

Set some default variables for both exporters.

```lisp
(setq
  user-full-name "Alexander Neville"
  org-export-with-archived-trees nil
  org-export-exclude-tags nil
  org-export-default-language nil
  org-export-preserve-breaks nil
  org-export-with-section-numbers nil
  org-export-select-tags nil
  org-export-with-author nil
  org-export-with-broken-links t
  org-export-with-clocks nil
  org-export-with-creator nil
  org-export-with-date nil
  org-export-with-drawers nil
  org-export-with-email nil
  org-export-with-emphasize t
  org-export-with-fixed-width nil
  org-export-with-footnotes nil
  org-export-with-latex t
  org-export-with-planning nil
  org-export-with-priority nil
  org-export-with-properties nil
  org-export-with-special-strings nil
  org-export-with-sub-superscripts '{}
  org-export-with-tables t
  org-export-with-tags nil
  org-export-with-tasks nil
  org-export-with-timestamps nil
  org-export-with-title nil
  org-export-with-toc nil
  org-export-with-todo-keywords nil
  )
```

In Org mode, it is possible to nest sections deeply, but in HTML and
LaTeX the number of available heading levels is limited. In HTML, the
tags `h1, ..., h6` are available, with `h1` being used for the document
title, leaving a maximum of 5 levels. In the default LaTeX _article_
class, `subsubsection` is the lowest heading level above paragraph,
which is 3 useful levels of nesting. The class should be changed to book
or report if greater heading depth is required.

```text
5. part           <- report/book
4. chapter
3. section        <- article
2. subsection
1. subsubsection
```

Set the maximum headline export level to 3, for the reasons given above.
This can be changed on a file-by-file basis.

```lisp
(setq org-export-headline-levels 3)
```

## LaTeX Exporter

LaTeX is a uniquely powerful typesetting system, though in many cases I
would rather focus on the content of my writing than worry about LaTeX
syntax. The more intuitive Org syntax with the ability to embed latex
fragments where required is a much better system.

Org must be configured to export documents to latex properly. Firstly,
the values of a number of variables must be considered when exporting to
LaTeX.

- `org-latex-default-packages-alist`: minimal preamble, best to leave
  unchanged.
- `org-latex-packages-alist`: empty by default, add preamble common to
  all Org documents.
- `org-latex-classes`: a list of ways to convert an Org-structured
  document to LaTeX.

The list `org-latex-classes` is very important. New classes can be added
with a _class-name_, _header-string_ and _sectioning-structure_ (see the
variable's help for more detailed information). In the header string you
can write a preamble, optionally including the contents of
`org-*-packages-alist`. So as to avoid repeating LaTeX fragments, a
custom preamble can be broken into sections and bind to variables.

```lisp
(setq
  chapter-redef
  "
  \\patchcmd{\\chapter}{\\thispagestyle{plain}}{\\thispagestyle{fancy}}{}{}
  \\makeatletter
  \\def\\@makechapterhead#1{
    \\vspace*{50\\p@}
    {\\parindent \\z@ \\raggedright \\normalfont
      \\ifnum \\c@secnumdepth >\\m@ne
          \\huge\\bfseries \\@chapapp\\space \\thechapter
          \\Huge\\bfseries \\thechapter.\\space%
          \\par\\nobreak
          \\vskip 20\\p@
      \\fi
      \\interlinepenalty\\@M
      \\Huge \\bfseries #1\\par\\nobreak
      \\vskip 40\\p@
    }}
  \\makeatother\n"
  report-fancyheader-def
  "
  \\usepackage{fancyhdr}
  \\pagestyle{fancy}
  \\renewcommand{\\sectionmark}[1]{\\markright{\\thesection~- ~#1}}
  \\renewcommand{\\chaptermark}[1]{\\markboth{\\chaptername~\\thechapter. \\textit{#1}}{}}
  \\fancyhf{}
  \\rfoot{page \\textbf{\\thepage}}
  \\lfoot{\\nouppercase{\\leftmark}}
  \\renewcommand{\\headrulewidth}{0pt}
  \\renewcommand{\\footrulewidth}{0.4pt}\n"
  article-fancyheader-def
  "
  \\usepackage{fancyhdr}
  \\pagestyle{fancy}
  \\fancyhf{}
  \\rfoot{page \\textbf{\\thepage}}
  \\lfoot{\\nouppercase{\\leftmark}}
  \\renewcommand{\\headrulewidth}{0pt}
  \\renewcommand{\\footrulewidth}{0.4pt}\n"
  common-head
  "
  \\usepackage{svg}
  \\svgsetup{inkscapelatex=false}
  \\usepackage{blindtext}
  \\usepackage{tcolorbox}
  \\usepackage{etoolbox}
  \\hypersetup{hidelinks}
  \\usemintedstyle{bw}
  \\setminted{autogobble=true, breaklines=true, breakbytokenanywhere=true, fontsize=\\small, xleftmargin=1cm, xrightmargin=1cm}
  \\usepackage[indent=0.5cm]{parskip}
  \\usepackage[a4paper, includefoot, margin=2.54cm]{geometry}\n"
  default-head-setup
  "
  \\usepackage[utf8]{inputenc}
  \\usepackage{libertine}
  \\usepackage{libertinust1math}
  \\usepackage[T1]{fontenc}
  \\usepackage{graphicx}
  \\usepackage{longtable}
  \\usepackage{wrapfig}
  \\usepackage{rotating}
  \\usepackage[normalem]{ulem}
  \\usepackage{amsmath}
  \\usepackage{amssymb}
  \\usepackage{capt-of}
  \\usepackage{hyperref}
  \\usepackage{minted}\n"
  )
```

Create each class as a list containing cons cells which represent the
headline levels of the latex document. Build the _header-string_ by
concatenating latex snippets together and add to the list. Finally, add
each user-defined class to `org-latex-classes`.

```lisp
(setq long-report   '(("\\part{%s}" . "\\part*{%s}")
                      ("\\chapter{%s}" . "\\chapter*{%s}")
                      ("\\section{%s}" . "\\section*{%s}")
                      ("\\subsection{%s}" . "\\subsection*{%s}")
                      ("\\subsubsection{%s}" . "\\subsubsection*{%s}"))
      short-report  '(("\\chapter{%s}" . "\\chapter*{%s}")
                      ("\\section{%s}" . "\\section*{%s}")
                      ("\\subsection{%s}" . "\\subsection*{%s}")
                      ("\\subsubsection{%s}" . "\\subsubsection*{%s}"))
      custom-article '(("\\section{%s}" . "\\section*{%s}")
                       ("\\subsection{%s}" . "\\subsection*{%s}")
                       ("\\subsubsection{%s}" . "\\subsubsection*{%s}")))

(setq report-common-header-string (concat "\\documentclass{report}\n[NO-DEFAULT-PACKAGES]\n[NO-PACKAGES]\n" default-head-setup chapter-redef common-head report-fancyheader-def "[EXTRA]"))
(add-to-list 'short-report report-common-header-string)
(add-to-list 'long-report report-common-header-string)
(add-to-list 'custom-article (concat "\\documentclass{article}\n[NO-DEFAULT-PACKAGES]\n[NO-PACKAGES]\n" default-head-setup common-head article-fancyheader-def "[EXTRA]"))
(add-to-list 'short-report "short-report")
(add-to-list 'long-report "long-report")
(add-to-list 'custom-article "custom-article")

(with-eval-after-load 'ox-latex
  (add-to-list 'org-latex-classes long-report)
  (add-to-list 'org-latex-classes short-report)
  (add-to-list 'org-latex-classes custom-article))

```

The following export options can be set in each Org document:

- `latex_class`: looked up in `org-latex-classes` to find the header to
  use.
- `latex_class_options`: document class options, could be rolled into
  the class header.
- `latex_header` & `latex_header_extra`: lines to be added to the header
  string for this file.

Use minted for exporting code blocks and set the preferred compiler. The
compiler can be changed with the Org option `latex_compiler`.

```lisp
(setq org-latex-listings 'minted
      org-latex-default-class "custom-article"
      org-export-in-background t
      org-latex-compiler "pdflatex"
      org-latex-pdf-process '("latexmk -f -pdf -%latex -shell-escape -interaction=nonstopmode -output-directory=%o %f"))
```

## HTML Exporter

Set the value of some variables.

```lisp
(setq org-html-doctype "html5"
  org-html-self-link-headlines t
  org-html-metadata-timestamp-format "%H:%M:%S %d/%m/%Y"
  org-html-creator-string "<a href=\"https://www.gnu.org/software/emacs/\">Emacs</a> 28.2 + <a href=\"https://orgmode.org\">Org mode</a> 9.5.5"
  org-html-head-include-default-style nil
  org-html-head-include-scripts nil
  )
```

This is a default MathJax configuration block. Its results are not used
as I overwrite the MathJax template in the block below.

```lisp
(setq org-html-mathjax-options
  '((path "https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0/MathJax.js?config=TeX-AMS_HTML")
    (scale "100")
    (align "center")
    (font "STIX-Web")
    (linebreaks "false")
    (autonumber "AMS")
    (indent "0em")
    (multlinewidth "85%")
    (tagindent ".8em")
    (tagside "right")))
```

Overwrite the template for inserting MathJax configuration to point to
my own copy of the scripts.

```lisp
(setq org-html-mathjax-template
  "
  <script>
  MathJax = {
    tex: { inlineMath: [['$', '$'], ['\\(', '\\)']] },
    svg: { fontCache: 'global' }
  };
  </script>
  <script type=\"text/javascript\" id=\"MathJax-script\" async
    src=\"https://cdn.afneville.com/mathjax/tex-svg.js\">
  </script>")
```

Additional lines for the head of each exported document. These lines are
given as an example.

```lisp
(setq org-html-head-extra
  "<link href=\"/res/style.css\" rel=\"stylesheet\" />
   <script src=\"/res/script.js\" defer></script>")
```

The _preamble_ and _postamble_ are inserted at the beginning and end of
the generated document respectively.

```lisp
(setq org-html-preamble
  "<header>
   <!-- ... -->
   </header>")

(setq org-html-postamble
  "<footer>
   <!-- ... -->
   </footer>")
```

## Publishing

The publishing feature of Org uses the configured exporters to convert
batches of files, according to some conditions.

```lisp
(setq main_html
  '("main_html"
    :recursive nil
    :base-directory "./src"
    :publishing-directory "./out"
    :base-extension "org"
    :publishing-function org-html-publish-to-html
    :with-title nil
    :with-toc nil
    :headline-levels 5
    )
  )

(setq main_res
  '("main_res"
    :recursive t
    :base-directory "./src/res"
    :publishing-directory "./out/res"
    :base-extension "pdf\\|jpg\\|gif\\|png\\|svg\\|css\\|js"
    :publishing-function org-publish-attachment)
```

With initialisation complete, each of the publishing functions is
executed.

```lisp
(setq org-publish-project-alist (append (list main_html) (list main_res)))
(org-publish-all t)
```
