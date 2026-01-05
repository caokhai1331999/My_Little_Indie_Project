git config --global --unset http.proxy
git pull --rebase
git add *.h *.cpp *.bat .emacs
git commit -m "latest update"
git push -u origin main
