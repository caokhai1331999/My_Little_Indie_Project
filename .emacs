;Melpa package function
(require 'package)
(add-to-list 'package-archives '("melpa" . "https://melpa.org/packages/") t)
;; Comment/uncomment this line to enable MELPA Stable if desired.
;; See `package-archive-priorities` and `package-pinned-packages`.
;; Most users will not need or want to do this.
;;(add-to-list 'package-archives
;;'("melpa-stable" . "http://stable.melpa.org/packages/") t)

(package-initialize)

(load-library "view")
(require 'cc-mode)
(require 'ido)
(require 'compile)
(ido-mode t)

;;This increases the maximum depth of the Lisp
;; evaluation stack to 10000
(setq max-lisp-eval-depth 10000)
;;(setq scroll-step 1)
(setq scroll-conservatively 10000)

;;Now change the lat and long titude
(setq calendar-latitude 12.25
      calendar-longitude 109.18333333333333)

;;set up circadian themes
(require 'circadian)
(setq circadian-themes '((:sunrise . solarized-selenized-light)
                         (:sunset . monokai)))
;;enable text mode
(add-to-list 'auto-mode-alist '("\\.txt\\'" . text-mode))
(setq next-line-add-newlines nil)
(setq-default truncate-lines t)
(setq truncate-partial-width-windows nil)
(split-window-horizontally)
(setq ediff-split-window-function 'split-window-horizontally)
(put 'upcase-region 'disabled 1)

;;(setq grep-program "findstr")
(setq grep-command "findstr /n /i /l /s *.cpp *.h")

					; C++ indentation style
(defconst casey-big-fun-c-style
  '((c-electric-pound-behavior   . nil)
    (c-tab-always-indent         . t)
    (c-comment-only-line-offset  . 0)
    (c-hanging-braces-alist      . ((class-open)
                                    (class-close)
                                    (defun-open)
                                    (defun-close)
                                    (inline-open)
                                    (inline-close)
                                    (brace-list-open)
                                    (brace-list-close)
                                    (brace-list-intro)
                                    (brace-list-entry)
                                    (block-open)
                                    (block-close)
                                    (substatement-open)
                                    (statement-case-open)
                                    (class-open)))
    (c-hanging-colons-alist      . ((inher-intro)
                                    (case-label)
                                    (label)
                                    (access-label)
                                    (access-key)
                                    (member-init-intro)))
    (c-cleanup-list              . (scope-operator
                                    list-close-comma
                                    defun-close-semi))
    (c-offsets-alist             . ((arglist-close         .  c-lineup-arglist)
                                    (label                 . -4)
                                    (access-label          . -4)
                                    (substatement-open     .  0)
                                    (statement-case-intro  .  4)
                                    (statement-block-intro .  c-lineup-for)
                                    (case-label            .  4)
                                    (block-open            .  0)
                                    (inline-open           .  0)
                                    (topmost-intro-cont    .  0)
                                    (knr-argdecl-intro     . -4)
                                    (brace-list-open       .  0)
                                    (brace-list-intro      .  4)))
    (c-echo-syntactic-information-p . t))
  "Casey's Big Fun C++ Style")

					; CC++ mode handling
(defun casey-big-fun-c-hook ()
					; Set my style for the current buffer
  (c-add-style "BigFun" casey-big-fun-c-style t)
  
					; 4-space tabs
  (setq tab-width 4
        indent-tabs-mode nil)

					; Additional style stuff
  (c-set-offset 'member-init-intro '++)

					; No hungry backspace
  (c-toggle-auto-hungry-state -1)

					; Newline indents, semi-colon doesn't
  (define-key c++-mode-map "\C-m" 'newline-and-indent)
  ;;(setq c-hanging-semi&comma-criteria '((lambda () 'stop)))

  ;; Enable show-paren-mode
  (show-paren-mode 1)

  ;; Set delay to zero
  (setq show-paren-delay 0)

  ;; Use parenthesis style
  (setq show-paren-style 'parenthesis)

  ;; Customize the face for highlighting
  (set-face-attribute 'show-paren-match 0
                      :background "#75715e"
                      :foreground "#a6e22e"
                      :weight 'bold)
  

					; Handle super-tabbify (TAB completes, shift-TAB actually tabs)
  (setq dabbrev-case-replace \t)
  (setq dabbrev-case-fold-search \t)
  (setq dabbrev-upcase-means-case-search \t)
  (define-key isearch-mode-map (kbd "TAB") 'dabbrev-expand)


					; Abbrevation expansion
  (abbrev-mode 1)
  
  (defun casey-header-format ()
    "Format the given file as a header file."
    (interactive)
    (setq BaseFileName (file-name-sans-extension (file-name-nondirectory buffer-file-name)))
    (insert "#if !defined(")
    (push-mark)
    (insert BaseFileName)
    (upcase-region (mark) (point))
    (pop-mark)
    (insert "_H)\n")
    (insert "/* ========================================================================\n")
    (insert "   $File: $\n")
    (insert "   $Date: $\n")
    (insert "   $Revision: $\n")
    (insert "   $Creator: Cao Khai(Casey's disciple) $\n")
    (insert "   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $\n")
    (insert "   ======================================================================== */\n")
    (insert "\n")
    (insert "#define ")
    (push-mark)
    (insert BaseFileName)
    (upcase-region (mark) (point))
    (pop-mark)
    (insert "_H\n")
    (insert "#endif")
    )

  (setq isearch-allow-scroll t)  ;; Allow scrolling while searching
  (setq isearch-allow-motion t)  ;; Allow arrow keys and normal editings

  ;;Set mark without highlighting
  (setq-default transient-mark-mode nil)


  ;; Little simple dynamic indent function
  ( defun dynamic-indent()
    (interactive)
    "indent region or line depend on situation"

    (if (and mark-active (and (/=(mark)(point)) (or (eolp)(bolp)))
	     (save-excursion (goto-char (mark))(or (eolp)(bolp))))
        (progn (indent-region (mark)(point))(message "indent region done"))
      (progn (indent-for-tab-command)(message "indent line done")))
    (if (transient-mark-mode) (transient-mark-mode 'toggle)(message "reset trasient mark"))
    )

					;Handle a little ; in the end and // at the beginning of line
  (defun ckhai-little-character-insert-fun()
    (interactive)
    "add ; at the end of line after hitting RET."
    ;;set current line as string
    (let ((line (thing-at-point 'line t)))
      (cond ((and (string-match "//" line)(eolp))
	     (newline-and-indent)
	     (insert "//"))
	    ((and (string-match ";" line)(eolp))
	     (newline-and-indent)
             (let ((linetwo (thing-at-point 'line t)))
               (unless (string-match ";" linetwo)
		 (insert ";")
		 (left-char 1))))
	    (t
	     (newline-and-indent))
	    )
      )
    )

  (defun casey-source-format ()
    "Format the given file as a source file."
    (interactive)
    (setq BaseFileName (file-name-sans-extension (file-name-nondirectory buffer-file-name)))
    (insert "/* ========================================================================\n")
    (insert "   $File: $\n")
    (insert "   $Date: $\n")
    (insert "   $Revision: $\n")
    (insert "   $Creator: Cao Khai(Casey Muratori's disciple) $\n")
    (insert "   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $\n")
    (insert "   ======================================================================== */\n")
    )

  (cond ((file-exists-p buffer-file-name) t)
        ((string-match "[.]hin" buffer-file-name) (casey-source-format))
        ((string-match "[.]cin" buffer-file-name) (casey-source-format))
        ((string-match "[.]h" buffer-file-name) (casey-header-format))
        ((string-match "[.]cpp" buffer-file-name) (casey-source-format)))

  (defun casey-find-corresponding-file ()
    "Find the file that corresponds to this one."
    (interactive)
    (setq CorrespondingFileName nil)
    (setq BaseFileName (file-name-sans-extension buffer-file-name))
    (if (string-match "\\.c" buffer-file-name)
	(setq CorrespondingFileName (concat BaseFileName ".h")))
    (if (string-match "\\.h" buffer-file-name)
	(if (file-exists-p (concat BaseFileName ".c")) (setq CorrespondingFileName (concat BaseFileName ".c"))
	  (setq CorrespondingFileName (concat BaseFileName ".cpp"))))
    (if (string-match "\\.hin" buffer-file-name)
	(setq CorrespondingFileName (concat BaseFileName ".cin")))
    (if (string-match "\\.cin" buffer-file-name)
	(setq CorrespondingFileName (concat BaseFileName ".hin")))
    (if (string-match "\\.cpp" buffer-file-name)
	(setq CorrespondingFileName (concat BaseFileName ".h")))
    (if CorrespondingFileName (find-file CorrespondingFileName)
      (error "Unable to find a corresponding file")))
  (defun casey-find-corresponding-file-other-window ()
    "Find the file that corresponds to this one."
    (interactive)
    (find-file-other-window buffer-file-name)
    (casey-find-corresponding-file)
    (other-window -1))
  (define-key c++-mode-map [f12] 'casey-find-corresponding-file)
  (define-key c++-mode-map [M-f12] 'casey-find-corresponding-file-other-window)

					; Alternate bindings for F-keyless setups (ie MacOS X terminal)
  (define-key c++-mode-map "\ec" 'casey-find-corresponding-file)
  (define-key c++-mode-map "\eC" 'casey-find-corresponding-file-other-window)

  (define-key c++-mode-map "\t" 'dabbrev-expand)

  (define-key c++-mode-map [S-tab] 'dynamic-indent)
  (define-key c++-mode-map "\C-y" 'indent-for-tab-command)
  (define-key c++-mode-map [C-tab] 'dynamic-indent)
  (define-key c++-mode-map (kbd "C-c i") 'indent-rigidly)
  (define-key c++-mode-map (kbd "RET") 'ckhai-little-character-insert-fun)

  (define-key c++-mode-map "\ej" 'imenu)

  (define-key c++-mode-map "\e." 'c-fill-paragraph)

  (define-key c++-mode-map "\e/" 'c-mark-function)

  (define-key c++-mode-map "\eq" 'append-as-kill)
  (define-key c++-mode-map "\ez" 'kill-region)

					; devenv.com error parsing
  (add-to-list 'compilation-error-regexp-alist 'casey-devenv)
  (add-to-list 'compilation-error-regexp-alist-alist '(casey-devenv
						       "*\\([0-9]+>\\)?\\(\\(?:[a-zA-Z]:\\)?[^:(\t\n]+\\)(\\([0-9]+\\)) : \\(?:see declaration\\|\\(?:warnin\\(g\\)\\|[a-z ]+\\) C[0-9]+:\\)"
						       2 3 nil (4)))
  )

(defun ckhai-replace-string (FromString ToString)
  "Replace a string without moving point."
  (interactive "sReplace: \nsReplace: %s  With: ")
  (save-excursion
    (replace-string FromString ToString)
    ))

(add-hook 'c-mode-common-hook 'casey-big-fun-c-hook)

					; TXT mode handling
(defun casey-big-fun-text-hook ()
					; 4-space tabs
  (setq tab-width 4
        indent-tabs-mode nil)

					; Newline indents, semi-colon doesn't
  (define-key text-mode-map "\C-m" 'newline-and-indent)

  )
(add-hook 'text-mode-hook 'casey-big-fun-text-hook)

;;Hide Tools bar
(tool-bar-mode 0)

;;Clock
(display-time)

;;Hide menu bar as well!!
(menu-bar-mode 0)

;;Set font
(set-face-attribute 'default nil :font "Fira Code" :height 95)

(defun Mark-Whole-Buffer()
  (interactive)
  (if (not transient-mark-mode)(transient-mark-mode 'toggle))
  (mark-whole-buffer)
  (message "Whole buffer marked"))

(defun custom-comment-func ()
  "Comment on c/c++ file format or batch code"
  (interactive)
  (when (buffer-file-name)
    (if (string-match-p "\\.\\(c\\|cpp\\|h\\)" buffer-file-name)
	;;whenever in c/c++ file
	(progn (if (and (or (use-region-p) (mark)) (/=(line-number-at-pos(mark)) (line-number-at-pos(point))))
		   ;;whenever region more than one line
		   (progn (let ((region-text (buffer-substring-no-properties (region-beginning) (region-end))))
			    ;; \\| mean OR
			    ;; special char like * must have \\ stand before
			    (progn (if (not (string-match-p "\\(/\\*\\|\\*/\\|//\\)" region-text))
				       (progn
					 (if (string-match-p ";" region-text)
					     (progn
					       (if (<(line-number-at-pos(point))(line-number-at-pos(mark)))
						   ;; But this didn't
						   ;; Why mark + 1 make this eternal loop
						   ;; bugs lied at next-line
						   (progn
						     (save-excursion  (while (< (line-number-at-pos(point))(+ (line-number-at-pos(mark)) 1)) (back-to-indentation)(insert "//")(forward-line 1))) )
						 ;; This case worked
						 (progn (save-excursion (while (>(+(line-number-at-pos (point)) 1)(line-number-at-pos (mark))) (back-to-indentation)(insert "//")(previous-line))))))
					   (progn 					 (if (> (line-number-at-pos(mark)) (line-number-at-pos(point)) )
											     ;;when mark is below point
											     (progn (save-excursion 
												      (insert "/*")
												      (goto-char (mark))
												      (insert "*/")))
											   ;;And vice-versa
											   (progn (save-excursion
												    (insert "*/")
												    (goto-char (mark))
												    (insert "/*")))
											   )))

					 ;;I just somehow put these reset inside of the point mark compare check block which is exclude out the mark above case
					 )
				     (progn
				       ;; Why this doesn't get in the block
				       (save-excursion (save-restriction (narrow-to-region (region-beginning)(region-end))
									 (goto-char (point-min))
									 ;; This Block pos still not change maybe the replace the delete function made it work
									 (while (re-search-forward "\\*/\\|/\\*\\|//" nil t)
									   (replace-match "")))
						       (let ((region-text (buffer-substring-no-properties (point-min) (point-max))))
							 (if (not (string-match-p "\\(/\\*\\|\\*/\\|//\\)" region-text))
							     (message "All comment cleaned")
							   (message "Still contains comment markers"))))
				       )
				     )
				   )
			    )
			  )
		 ;; when there are one line use built-in one
		 (progn
		   (back-to-indentation)
		   (let ((line-text (buffer-substring-no-properties (line-beginning-position) (line-end-position))))
		     (if (not (string-match-p "//" line-text))(progn (back-to-indentation) (insert "//"))(progn (save-excursion (save-restriction (narrow-to-region (line-beginning-position) (line-end-position)) (perform-replace "//" "" nil nil nil)))))))
		 )	 
	       ;; Right in the c/cpp file extension condition 1st progn
	       (if (transient-mark-mode)(progn (transient-mark-mode 'toggle) (message "Trasient mark mode is reseted")) (progn (message "There is no activated trasient mark")))
	       (if (mark) (progn (activate-mark nil) (set-mark nil) (message "mark is just reseted"))
		 (progn (message "mark is already reseted")))
	       )
      ;;other format just use built-in comment
      (progn (comment-dwim nil)))))

(defun clone-region-or-line ()
  "Clone the highlighted region if active, otherwise clone the current line."
  (interactive)
  (if (and (mark)(and (/= (mark) (point))(or (bolp)(eolp)))(save-excursion (goto-char (mark))(or (eolp)(bolp))))
      (progn  (let ((start (min (point) (mark)))
		    (end (max (point) (mark))))

		(kill-ring-save start end)
		(goto-char end)
		(newline)
		(yank)
		(move-beginning-of-line nil)
		(message "region cloned")))

    (progn (kill-ring-save (point-at-bol) (point-at-eol))
	   (move-end-of-line nil)
	   (newline)
	   (yank)
	   (move-beginning-of-line nil)
	   (message "line cloned"))))

(defun isearch-or-repeat ()
  "Search forward and repeat when hitting f3"
  (interactive)
  (if (and isearch-string (not (string= isearch-string "")))
      (progn (isearch-mode t)
	     (isearch-yank-string isearch-string)
	     (isearch-repeat-forward))
    (progn (isearch-mode t)
	   (isearch-edit-string)
	   (isearch-repeat-forward))))


(defun dynamic-copy ()
  "Copy line or region depends on situation go through all situation that about region"
  (interactive)
  (if (transient-mark-mode)(transient-mark-mode 'toggle))
  (if (or (use-region-p) (and (mark)(/=(point)(mark))) )
      (progn
        (let ((start (min (point) (mark)))
              (end (max (point) (mark))))
          (kill-ring-save start end))
	(message "Region copied and reset transient mark")
	(activate-mark nil)	  
	(set-mark nil))
    ;;NOTE: In lisp emacs the else block have to be inside the if block
    (progn
      (set-mark nil)
      (kill-ring-save (line-beginning-position) (line-end-position))
      (message "line copied reset transient mark"))))

;;My backward-kill-word function
"Delete the previous word without adding it to the kill ring."
(defun my-backward-kill-word ()
  "Delete whitespace characters or words before point."
  (interactive)
  (save-excursion
      ;; So eq is the equality test(compare fx) and ?\s present for white space
      (if (eq (char-before) ?\s) (delete-horizontal-space))
      ;; litterally non-space+space or space+non-space char
      (if(looking-back "[^0-9a-zA-Z]" 1)(backward-delete-char 1))
      (let ((start (point)))
      (skip-chars-backward "0-9A-Za-z\t" (line-beginning-position))
	(delete-region (point) start))))
;;)

(defun append-rectangles-side-by-side (start1 end1 start2 end2)
  "Append rectangle between START1 and END1 to the right of rectangle between START2 and END2."
  (interactive "r\nr")
  (let ((rect1 (extract-rectangle start1 end1))
        (rect2 (extract-rectangle start2 end2)))
    (save-excursion
      (goto-char (min start2 end2))
      (cl-mapcar
       (lambda (line1 line2)
         (insert (concat line2 "   " line1) "\n")
         (delete-region (line-beginning-position) (line-end-position))
         (forward-line 1))
       rect1 rect2))))

;;Move line function
(defun move-line (n)
  "Move the current line up or down by N lines."
  (interactive "p")
  (setq col (current-column))
  (beginning-of-line) (setq start (point))
  (end-of-line) (forward-char) (setq end (point))
  (let ((line-text (delete-and-extract-region start end)))
    (forward-line n)
    (insert line-text)
    ;; restore point to original column in moved line
    (forward-line -1)
    (forward-char col)))

(defun move-line-up (n)
  "Move the current line up by N lines."
  (interactive "p")
  (move-line (if (null n) -1 (- n))))

(defun move-line-down (n)
  "Move the current line down by N lines."
  (interactive "p")
  (move-line (if (null n) 1 n)))

;;Move text function
(defun move-text-internal (arg)
  (cond
   ((and mark-active transient-mark-mode)
    (if (> (point) (mark))
        (exchange-point-and-mark))
    (let ((column (current-column))
          (text (delete-and-extract-region (point) (mark))))
      (forward-line arg)
      (move-to-column column t)
      (set-mark (point))
      (insert text)
      (exchange-point-and-mark)
      (setq deactivate-mark nil)))
   (t
    (let ((column (current-column)))
      (beginning-of-line)
      (when (or (> arg 0) (not (bobp)))
        (forward-line)
        (when (or (< arg 0) (not (eobp)))
          (transpose-lines arg)
          (when (and (eval-when-compile
		       '(and (>= emacs-major-version 24)
                             (>= emacs-minor-version 3)))
                     (< arg 0))
            (forward-line -1)))
        (forward-line -1))
      (move-to-column column t)))))

(defun move-text-down (arg)
  "Move region (transient-mark-mode active) or current line
  arg lines down."
  (interactive "*p")
  (move-text-internal arg))

(defun move-text-up (arg)
  "Move region (transient-mark-mode active) or current line
  arg lines up."
  (interactive "*p")
  (move-text-internal (- arg)))

;; Bright-red TODOs, BRIGHT DONE!, WORKING
(setq fixme-modes '(c++-mode c-mode text-mode))
(make-face 'font-lock-fixme-face)
(make-face 'font-lock-note-face)
(make-face 'font-lock-jobDone-face)
(make-face 'font-lock-working-face)
(make-face 'font-lock-remember-face)
(make-face 'font-lock-solved-face)
(modify-face 'font-lock-fixme-face "Red" nil nil t nil t nil nil)
(modify-face 'font-lock-note-face "goldenrod" nil nil t nil t nil nil)
(modify-face 'font-lock-jobDone-face "rgb:ff/82/47" nil nil t nil t nil nil)
(modify-face 'font-lock-working-face "lime green" nil nil t nil t nil nil)
(modify-face 'font-lock-solved-face "green yellow" nil nil t nil t nil nil)
(modify-face 'font-lock-remember-face "rgb:ba/a0/f8" nil nil t nil t nil nil)

;; Define a function to add keywords
(defun my-add-fixme-keywords ()
  (font-lock-add-keywords
   nil
   '(("\\<\\(TODO:\\)" 1 'font-lock-fixme-face t)
     ("\\<\\(NOTE:\\)" 1 'font-lock-note-face t)
     ("\\<\\(DONE!\\)" 1 'font-lock-jobDone-face t)
     ("\\<\\(WORKING ON IT!!\\)" 1 'font-lock-working-face t)
     ("\\<\\(SOLVED!\\)" 1 'font-lock-solved-face t)
     ("\\<\\(REMEMBER! :\\)" 1 'font-lock-remember-face t))
   'append)) ;; use 'append to not overwrite existing highlighting

;; Set up for different modes
(dolist (mode '(text-mode c++-mode c-mode emacs-lisp-mode))
  (let ((hook (intern (concat (symbol-name mode) "-hook"))))
    (add-hook hook #'my-add-fixme-keywords)))

;;========================================================================================
;; Enable visual-line-mode globally
(global-visual-line-mode 1)

;; Set the default fill column
(setq-default fill-column 80)

;; Ensure visual-line-mode and auto-fill-mode are enabled in text-mode and org-mode
(add-hook 'text-mode-hook 'visual-line-mode)
(add-hook 'text-mode-hook 'auto-fill-mode)
(add-hook 'org-mode-hook 'visual-line-mode)
(add-hook 'org-mode-hook 'auto-fill-mode)
;;=========================================================================================

;;lock compilation dir func
(defun lock-compilation-directory ()
  "The compilation process should NOT hunt for a makefile"
  (interactive)
  (setq compilation-directory-locked t)
  (message "Compilation directory is locked."))

;;unlock compilation dir
(defun unlock-compilation-directory ()
  "The compilation process SHOULD hunt for a makefile"
  (interactive)
  (setq compilation-directory-locked nil)
  (message "Compilation directory is roaming."))

;;make script
(setq my-makescript "build.bat")
;;Another makefile
(setq my-makescript1 "makefile")

;;Find project dir function
(defun find-project-directory ()
  "Find the project directory."
  (interactive)
  (setq find-project-from-directory default-directory)
  (switch-to-buffer-other-window "*compilation*")
  (if compilation-directory-locked (cd last-compilation-directory)
    (cd find-project-from-directory)
    (find-project-directory-recursive)
    (setq last-compilation-directory default-directory)))

;;recursively search for makefile
(defun find-project-directory-recursive ()
  "Recursively search for a makefile."
  (interactive)
  (if (file-exists-p my-makescript) t
    (cd "../")
    (find-project-directory-recursive)
    ;;if (file-exists-p my-makescript) t
    ;;(cd "../")
    ;;(find-project-directory-recursive)
    ))

;;Set compilation-dir-lock to open
(setq compilation-directory-locked nil)

;;Make with out asking function
(defun make-without-asking ()
  "Make the current build."
  (interactive)
  (if (find-project-directory) (compile my-makescript))
  (other-window 1))
(define-key global-map "\em" 'make-without-asking)

(setq todo-file "./Todo.txt")
(setq log-file "./log.txt")
(setq issue-file "./ArisedIssues.txt")

(defun insert-timeofday ()
  (interactive "*")
  (insert (format-time-string "---------------- %a, %d %b %y: %I:%M%p")))

(defun load-todo()
  (interactive)
  (if (file-exists-p todo-file)
      (progn (find-file todo-file))
    (progn (cd "../")
	   (find-file todo-file)))
  (end-of-buffer)
  (newline-and-indent)
  (insert-timeofday)
  (newline-and-indent)
  (newline-and-indent))

(defun load-log ()
  (interactive)
  (if (file-exists-p log-file)
      (progn (find-file log-file))
    (progn (cd "../")
	   (find-file log-file)))
  (end-of-buffer) 
  
  (newline-and-indent)
  (insert-timeofday)
  (newline-and-indent)
  (newline-and-indent)
  (end-of-buffer)
  )

(defun load-issue ()
  (interactive)
  (if (file-exists-p issue-file)
      (progn (find-file issue-file))
    (progn (cd "../")
	   (find-file issue-file)))
  (end-of-buffer)
  (newline-and-indent)
  (insert-timeofday)
  (newline-and-indent)
  (newline-and-indent)
  (end-of-buffer)
  )


					; Accepted file extensions and their appropriate modes
(setq auto-mode-alist
      (append
       '(("\\.cpp$"    . c++-mode)
         ("\\.hin$"    . c++-mode)
         ("\\.cin$"    . c++-mode)
         ("\\.inl$"    . c++-mode)
         ("\\.rdc$"    . c++-mode)
         ("\\.h$"    . c++-mode)
         ("\\.c$"   . c++-mode)
         ("\\.cc$"   . c++-mode)
         ("\\.c8$"   . c++-mode)
         ("\\.txt$" . indented-text-mode)
         ("\\.emacs$" . emacs-lisp-mode)
         ("\\.gen$" . gen-mode)
         ("\\.ms$" . fundamental-mode)
         ("\\.m$" . objc-mode)
         ("\\.mm$" . objc-mode)
         ) auto-mode-alist))


;;intellij-style smart backward-kill-word


;;Keybindings
					;file
(define-key global-map "\et" 'load-todo)
(define-key global-map "\eL" 'load-log)
(define-key global-map "\ei" 'load-issue)
(define-key global-map "\ef" 'find-file)
(define-key global-map "\eF" 'find-file-other-window)
					;buffer
(global-set-key (kbd "\ew") 'other-window)
(global-set-key (read-kbd-macro "\eb")  'ido-switch-buffer)
(global-set-key (read-kbd-macro "\eB")  'ido-switch-buffer-other-window)
(define-key global-map "\er" 'revert-buffer)
(define-key global-map "\ek" 'kill-this-buffer)

;;move cursor
(define-key global-map [prior] 'beginning-of-buffer)
(define-key global-map [next] 'end-of-buffer)
(global-unset-key [M-left])
(global-set-key [M-left] (lambda () (interactive) (if (bolp) (progn (forward-line -1)) (progn (move-beginning-of-line 1)))))
(global-unset-key [M-right])
(global-set-key [M-right] (lambda () (interactive) (if (eolp) (progn (forward-line 1) (move-end-of-line 1)) (progn (move-end-of-line 1)))))


					;edit
(global-unset-key (kbd "C-x h"))
(define-key global-map "\C-A" 'Mark-Whole-Buffer)
(global-unset-key (kbd "C-y"))
(global-set-key (kbd "C-y") 'scroll-up-command)
(global-unset-key (kbd "C-v"))
(global-set-key (kbd "C-v") 'yank)

; Column Text edit
(global-unset-key (kbd "C-r"))
(global-set-key (kbd "C-r c") 'copy-rectangle-as-kill)
(global-unset-key (kbd "C-r k"))
(global-set-key (kbd "C-r k") 'kill-rectangle)
(global-unset-key (kbd "C-r y"))
(global-set-key (kbd "C-r y") 'yank-rectangle)
(global-unset-key (kbd "C-r r"))
(global-set-key (kbd "C-r r") 'string-rectangle)

;;(global-unset-key (kbd "C-s r"))
;;(global-unset-key (kbd "C-s r") 'append-rectangles-side-by-side)

;;Search
(global-unset-key (kbd "C-f"))
(global-set-key (kbd "C-f") 'grep)
(global-set-key (kbd "<f3>") 'isearch-repeat-forward)
(global-set-key (kbd "<f3>") 'isearch-or-repeat)
(global-set-key (kbd "S-<f3>") 'isearch-repeat-backward)

;;Replace string
(global-unset-key (kbd "C-x r"))
(global-set-key (kbd "C-x r") 'replace-string)

(global-set-key (kbd "C-s") 'save-buffer)
(define-key global-map (kbd "C-l") 'clone-region-or-line)
(define-key global-map (kbd "C-p") 'dynamic-copy)
(define-key global-map (kbd "M-p") 'previous-line)

(global-unset-key (kbd "C-x o"))
(global-unset-key (kbd "C-<backspace>"))
(global-unset-key (kbd "M-<backspace>"))

					;comment
(global-unset-key (kbd "M-;"))
(global-set-key (kbd "M-;") 'custom-comment-func)

					;move text around
					;(global-set-key [S-backspace] 'phg/kill-to-bol)
(global-set-key [M-S-up] 'move-text-up)
(global-set-key [M-S-down] 'move-text-down)
(global-set-key [M-up] 'move-line-up)
(global-set-key [M-down] 'move-line-down)
					;other
(global-set-key (kbd "C-<backspace>") 'my-backward-kill-word)
(global-set-key (kbd "M-<backspace>") 'backward-kill-sentence)
(global-set-key (kbd "C-h") 'shell)
(global-set-key (kbd "C-z") 'undo)
(global-set-key (kbd "C-x u") 'suspend-frame)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(ansi-color-faces-vector
   [default default default italic underline success warning error])
 '(ansi-color-names-vector
   ["#2d3743" "#ff4242" "#74af68" "#dbdb95" "#34cae2" "#008b8b" "#00ede1" "#e1e1e0"])
 '(beacon-color "#cc6666")
 '(company-quickhelp-color-background "#4F4F4F")
 '(company-quickhelp-color-foreground "#DCDCCC")
 '(compilation-message-face (quote default))
 '(cua-global-mark-cursor-color "#3cafa5")
 '(cua-normal-cursor-color "#8d9fa1")
 '(cua-overwrite-cursor-color "#c49619")
 '(cua-read-only-cursor-color "#93a61a")
 '(custom-enabled-themes (quote (monokai)))
 '(custom-safe-themes
   (quote
    ("443e2c3c4dd44510f0ea8247b438e834188dc1c6fb80785d83ad3628eadf9294" "56044c5a9cc45b6ec45c0eb28df100d3f0a576f18eef33ff8ff5d32bac2d9700" "4fda8201465755b403a33e385cf0f75eeec31ca8893199266a6aeccb4adedfa4" "60ada0ff6b91687f1a04cc17ad04119e59a7542644c7c59fc135909499400ab8" "00cec71d41047ebabeb310a325c365d5bc4b7fab0a681a2a108d32fb161b4006" "3fe1ebb870cc8a28e69763dde7b08c0f6b7e71cc310ffc3394622e5df6e4f0da" "b54376ec363568656d54578d28b95382854f62b74c32077821fdfd604268616a" "b99e334a4019a2caa71e1d6445fc346c6f074a05fcbb989800ecbe54474ae1b0" "a9abd706a4183711ffcca0d6da3808ec0f59be0e8336868669dc3b10381afb6f" "8d8207a39e18e2cc95ebddf62f841442d36fcba01a2a9451773d4ed30b632443" "a138ec18a6b926ea9d66e61aac28f5ce99739cf38566876dc31e29ec8757f6e2" "2dd4951e967990396142ec54d376cced3f135810b2b69920e77103e0bcedfba9" "6945dadc749ac5cbd47012cad836f92aea9ebec9f504d32fe89a956260773ca4" "7a424478cb77a96af2c0f50cfb4e2a88647b3ccca225f8c650ed45b7f50d9525" "2e05569868dc11a52b08926b4c1a27da77580daa9321773d92822f7a639956ce" "be84a2e5c70f991051d4aaf0f049fa11c172e5d784727e0b525565bb1533ec78" "fee7287586b17efbfda432f05539b58e86e059e78006ce9237b8732fde991b4c" "7f1d414afda803f3244c6fb4c2c64bea44dac040ed3731ec9d75275b9e831fe5" "06f0b439b62164c6f8f84fdda32b62fb50b6d00e8b01c2208e55543a6337433a" "628278136f88aa1a151bb2d6c8a86bf2b7631fbea5f0f76cba2a0079cd910f7d" "bb08c73af94ee74453c90422485b29e5643b73b05e8de029a6909af6a3fb3f58" "1b8d67b43ff1723960eb5e0cba512a2c7a2ad544ddb2533a90101fd1852b426e" "82d2cac368ccdec2fcc7573f24c3f79654b78bf133096f9b40c20d97ec1d8016" "f5b6be56c9de9fd8bdd42e0c05fecb002dedb8f48a5f00e769370e4517dde0e8" "57a29645c35ae5ce1660d5987d3da5869b048477a7801ce7ab57bfb25ce12d3e" "833ddce3314a4e28411edf3c6efde468f6f2616fc31e17a62587d6a9255f4633" "d89e15a34261019eec9072575d8a924185c27d3da64899905f8548cbd9491a36" default)))
 '(debug-on-error t)
 '(exwm-floating-border-color "#262626")
 '(fci-rule-color "#383838")
 '(flycheck-color-mode-line-face-to-color (quote mode-line-buffer-id))
 '(frame-background-mode (quote dark))
 '(highlight-changes-colors (quote ("#e2468f" "#7a7ed2")))
 '(highlight-symbol-colors
   (quote
    ("#3c6f408d329d" "#0c4a45f64ce3" "#486e33913532" "#1fac3bea568d" "#2ec943ac3324" "#449935a7314d" "#0b04411b5986")))
 '(highlight-symbol-foreground-color "#9eacac")
 '(highlight-tail-colors
   (quote
    (("#01323d" . 0)
     ("#687f00" . 20)
     ("#008981" . 30)
     ("#0069b0" . 50)
     ("#936d00" . 60)
     ("#a72e01" . 70)
     ("#a81761" . 85)
     ("#01323d" . 100))))
 '(hl-bg-colors
   (quote
    ("#936d00" "#a72e01" "#ae1212" "#a81761" "#3548a2" "#0069b0" "#008981" "#687f00")))
 '(hl-fg-colors
   (quote
    ("#002732" "#002732" "#002732" "#002732" "#002732" "#002732" "#002732" "#002732")))
 '(hl-paren-colors (quote ("#3cafa5" "#c49619" "#3c98e0" "#7a7ed2" "#93a61a")))
 '(jdee-db-active-breakpoint-face-colors (cons "#000000" "#8CD0D3"))
 '(jdee-db-requested-breakpoint-face-colors (cons "#000000" "#7F9F7F"))
 '(jdee-db-spec-breakpoint-face-colors (cons "#000000" "#494949"))
 '(lsp-ui-doc-border "#9eacac")
 '(magit-diff-use-overlays nil)
 '(nrepl-message-colors
   (quote
    ("#CC9393" "#DFAF8F" "#F0DFAF" "#7F9F7F" "#BFEBBF" "#93E0E3" "#94BFF3" "#DC8CC3")))
 '(objed-cursor-color "#CC9393")
 '(package-selected-packages (quote (circadian doom-themes solarized-theme)))
 '(pdf-view-midnight-colors (quote ("#DCDCCC" . "#383838")))
 '(pos-tip-background-color "#01323d")
 '(pos-tip-foreground-color "#9eacac")
 '(rustic-ansi-faces
   ["#3F3F3F" "#CC9393" "#7F9F7F" "#F0DFAF" "#8CD0D3" "#DC8CC3" "#93E0E3" "#DCDCDC"])
 '(smartrep-mode-line-active-bg (solarized-color-blend "#93a61a" "#01323d" 0.2))
 '(term-default-bg-color "#002732")
 '(term-default-fg-color "#8d9fa1")
 '(vc-annotate-background "#2B2B2B")
 '(vc-annotate-background-mode nil)
 '(vc-annotate-color-map
   (quote
    ((20 . "#BC8383")
     (40 . "#CC9393")
     (60 . "#DFAF8F")
     (80 . "#D0BF8F")
     (100 . "#E0CF9F")
     (120 . "#F0DFAF")
     (140 . "#5F7F5F")
     (160 . "#7F9F7F")
     (180 . "#8FB28F")
     (200 . "#9FC59F")
     (220 . "#AFD8AF")
     (240 . "#BFEBBF")
     (260 . "#93E0E3")
     (280 . "#6CA0A3")
     (300 . "#7CB8BB")
     (320 . "#8CD0D3")
     (340 . "#94BFF3")
     (360 . "#DC8CC3"))))
 '(vc-annotate-very-old-color "#DC8CC3")
 '(weechat-color-list
   (quote
    (unspecified "#002732" "#01323d" "#ae1212" "#ec423a" "#687f00" "#93a61a" "#936d00" "#c49619" "#0069b0" "#3c98e0" "#a81761" "#e2468f" "#008981" "#3cafa5" "#8d9fa1" "#60767e")))
 '(window-divider-mode nil)
 '(xterm-color-names
   ["#01323d" "#ec423a" "#93a61a" "#c49619" "#3c98e0" "#e2468f" "#3cafa5" "#faf3e0"])
 '(xterm-color-names-bright
   ["#002732" "#db5823" "#62787f" "#60767e" "#8d9fa1" "#7a7ed2" "#9eacac" "#ffffee"])
 
 ;;Auto Complete parenthesises
 '(electric-pair-mode 1)
 '(setq electric-pair-preserve-balance nil)

 ;;Auto highlight line
 '(global-hl-line-mode 1)

 ;;Start with debug init
 '(toggle-debug-on-error)

 '(auto-save-default nil)
 '(auto-save-interval 0)
 '(auto-save-list-file-prefix nil)
 '(auto-save-timeout 0)
 '(auto-show-mode t t)
 '(delete-auto-save-files nil)
 '(delete-old-versions (quote other))
 '(imenu-auto-rescant t)
 '(imenu-auto-rescan-maxout 500000)
 '(kept-new-versions 5)
 '(kept-old-versions 5)
 '(make-backup-file-name-function (quote ignore))
 '(make-backup-files nil)
 '(mouse-wheel-follow-mouse nil)
 '(mouse-wheel-progressive-speed nil)
 '(mouse-wheel-scroll-amount (quote (12)))
 '(version-control nil))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

;;Trigger circadian-setup
(circadian-setup)

;;Menu bar custom and title bar
(custom-set-faces
 `(menu ((t (:background "#2e2e2e" :foreground "#FFB2F9")))))

;; Enable transparent scrollbar
;;(set-face-attribute 'vertical-scroll-bar nil :background "none")
(scroll-bar-mode -1)
(put 'scroll-left 'disabled nil)
