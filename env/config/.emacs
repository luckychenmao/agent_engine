(setq locale-coding-system'utf-8)
(prefer-coding-system'utf-8)
(set-keyboard-coding-system'utf-8)
(set-terminal-coding-system'utf-8)
(set-selection-coding-system'utf-8)
(set-clipboard-coding-system 'ctext)
(set-buffer-file-coding-system 'utf-8)

(require 'package) ;; You might already have this line
(add-to-list 'package-archives
             '("melpa" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/melpa/") t)
(add-to-list 'package-archives
             '("melpa" . "https://melpa.org/packages/") t)
(package-initialize)

(setq grep-command "grep --exclude=\"*\\.svn*\" -nHi -e ")
(setq grep-cpp-command "grep --include=\"*\\.cpp\" --include=\"*\\.cc\" --include=\"*\.h\" --exclude=\"*Test\.cpp\" -r -nHi -e ")
(setq grep-proto-command "grep --include=\"*\\.proto\" -r -nHi -e ")
(setq grep-bazel-build-command "grep  --include=\"BUILD\" -r -nHi -e ")
(setq grep-etest-command "grep --exclude=\"*/test*\" --include=\"*\\.cpp\" --include=\"*\.h\" \. -r -nHi -e ")
(setq grep-py-command "grep --include=\"*\\.py\"  -r -nHi ")

(global-set-key [remap save-buffers-kill-terminal] #'save-buffers-kill-emacs)
(setq confirm-kill-emacs #'yes-or-no-p)

(global-set-key  (kbd "C-x TAB") 'dabbrev-expand)
(global-set-key  (kbd "C-x l") 'windmove-left)
(global-set-key  (kbd "C-x r") 'windmove-right)
(global-set-key  (kbd "C-x p") 'windmove-up)
(global-set-key  (kbd "C-x d") 'windmove-down)
(global-set-key  (kbd "C-x C-b") 'magit-log-buffer-file)
(global-set-key  (kbd "C-x C-b") 'magit-log-buffer-file)
(global-set-key [f5] 'refresh-buffer)
(global-set-key [f6] 'my-compile)
(global-set-key [f7] 'eshell)
(global-set-key [f8] 'open-eshell-other-buffer)
(global-set-key [f9] 'compile)
(load "/usr/share/emacs/site-lisp/site-start.d/clang-format.el")
(global-set-key [f11] 'clang-format-region)
(global-set-key [f12] 'clang-format-buffer)

;;compilation settings
(defun my-compile()
  "Save buffers and start compile"
  (interactive)
  (save-some-buffers t)
  (switch-to-buffer-other-window "*compilation*")
  (compile compile-command))

;; magit
(defun call-magit-status()
  "open magit status"
  (interactive) (call-interactively 'magit-status)
  )
(global-set-key (kbd "C-x g") 'call-magit-status)

;;eshell settings
(defun open-eshell-other-buffer()
  "Open eshell in other buffer"
  (interactive)
  (split-window-vertically)
  (eshell))

;;目录推荐
(require 'ido)
(ido-mode 1)
(setq ido-enable-flex-matching t)  ;; 允许模糊匹配
(setq ido-everywhere t)            ;; 启用所有可能地方的 ido
(setq ido-auto-merge-work-directories-length -1) ;; 不自动合并目录
(setq ido-use-filename-at-point 'guess)          ;; 从光标位置猜测文件名

;;(helm-mode 1)
;; (custom-set-variables
;;  ;; custom-set-variables was added by Custom.
;;  ;; If you edit it by hand, you could mess it up, so be careful.
;;  ;; Your init file should contain only one such instance.
;;  ;; If there is more than one, they won't work right.
;;  '(package-selected-packages '(standard-themes helm)))

;; (load-theme 'standard-dark t) ;; 或 'standard-light

;; (global-set-key (kbd "C-x C-f") 'helm-find-files)

(global-set-key (kbd "M-i") 'find-file-at-point)

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(magit-branch-current ((t (:inherit magit-branch-local :box 1))))
 '(magit-diff-added ((t (:foreground "#22aa22"))))
 '(magit-diff-added-highlight ((t (:foreground "#22aa22"))))
 '(magit-diff-base ((t (:background "#ffffcc" :foreground "#aaaa11"))))
 '(magit-diff-base-highlight ((t (:background "#eeeebb" :foreground "#aaaa11"))))
 '(magit-diff-conflict-heading ((t nil)))
 '(magit-diff-context ((t nil)))
 '(magit-diff-context-highlight ((t nil)))
 '(magit-diff-file-heading ((t (:weight bold))))
 '(magit-diff-file-heading-highlight ((t (:inherit magit-section-highlight))))
 '(magit-diff-file-heading-selection ((t (:inherit magit-diff-file-heading-highlight :foreground "salmon4"))))
 '(magit-diff-hunk-heading ((t nil)))
 '(magit-diff-hunk-heading-highlight ((t nil)))
 '(magit-diff-hunk-heading-selection ((t (:inherit magit-diff-hunk-heading-highlight :foreground "salmon4"))))
 '(magit-diff-lines-boundary ((t (:inherit magit-diff-lines-heading))))
 '(magit-diff-lines-heading ((t (:inherit magit-diff-hunk-heading-highlight :background "LightSalmon3"))))
 '(magit-diff-our ((t (:inherit magit-diff-removed))))
 '(magit-diff-our-highlight ((t nil)))
 '(magit-diff-removed ((t (:foreground "#aa2222"))))
 '(magit-diff-removed-highlight ((t (:foreground "#aa2222"))))
 '(magit-diff-their ((t (:inherit magit-diff-added))))
 '(magit-diff-their-highlight ((t (:inherit magit-diff-added-highlight))))
 '(magit-dimmed ((t (:foreground "grey50"))))
 '(magit-hash ((t (:foreground "green"))))
 '(magit-item-highlight ((t (:inherit nil))))
 '(magit-section-highlight ((t nil)))
 '(magit-section-secondary-heading ((t (:foreground "green" :weight normal))))
 '(region ((t (:background "blue" :foreground "green")))))

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(package-selected-packages '(bazel clang-format magit standard-themes helm)))


(add-hook 'before-save-hook 'delete-trailing-whitespace)


;;c++ related
(add-hook 'c++-mode-hook
	  '(lambda ( )
	     (c-toggle-hungry-state)))
(add-hook 'c++-mode-hook
	  '(lambda ( )
	     (flyspell-prog-mode)))

(add-hook 'c++-mode-hook
	  '(lambda ( )
	     (c-set-style "vespa")))

(setq auto-mode-alist
      (cons '("\\.h\\'" . c++-mode) auto-mode-alist))

(add-to-list 'auto-mode-alist
	     '("\\.pp\\'" . c++-mode))

(add-to-list 'auto-mode-alist
	     '("\\.js\\'" . java-mode))

(add-to-list 'auto-mode-alist
	     '("\\.cava\\'" . java-mode))
;; (add-to-list 'auto-mode-alist
;; '("\\.ll\\'" . c++-mode))

;; (add-to-list 'auto-mode-alist
;; '("\\.yy\\'" . c++-mode))

(add-to-list 'auto-mode-alist
	     '("\\.proto\\'" . java-mode))

;;c-mode related
(add-hook 'c-mode-hook
	  '(lambda ( )
	     (c-toggle-hungry-state)))
(add-hook 'c-mode-hook
	  '(lambda ( )
	     (flyspell-prog-mode)))

(add-to-list 'auto-mode-alist
	     '("\\BUILD\\'" . python-mode))


;; enable visual feedback on selections
					;(setq transient-mark-mode t)

;; default to better frame titles
(setq frame-title-format (concat  "%b - emacs@" (system-name)))

;; default to unified diffs
(setq diff-switches "-u")

;;close bell
(setq visible-bell t)

;;not insert tab
(setq-default indent-tabs-mode nil)

;;(require 'linum+)
;;(global-linum-mode 1)
;;(setq linum-format "%d| ")

;;show column number
(column-number-mode t)

;; hide menu bar
(menu-bar-mode -1)

;; high light
(show-paren-mode t)
(setq show-paren-style 'parentheses)

;;transient-mark-mode
(setq transient-mark-mode t)

;;turn on highlighting current line
;;(global-hl-line-mode 1)

;; for buffer
(global-set-key (kbd "M-n") 'next-buffer)
(global-set-key (kbd "M-p") 'previous-buffer)

;; not generate ~ temp file
(setq make-backup-files nil)

;; for c mode dev
(setq c-basic-offset 4)

(setq ispell-dictionary "en")
