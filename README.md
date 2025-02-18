# COMP310 Environment Setup

This project supports multiple development environments (Windows, WSL, and MIMI). 
Follow these instructions to switch between environments.
(Albeit the manual setup sucks, but i couldn't figure out a more elegant workflow)

## Manual Switching (if scripts don't work)

If the scripts don't work for any reason, you can manually switch your environment by:

1. Renaming your current `.vscode` folder to its appropriate environment name (e.g., `.vscode-win64`, `.vscode-wsl`, or `.vscode-mimi`)
2. Renaming the desired environment folder to `.vscode`

For example, to switch to WSL:
```bash
mv .vscode-wsl .vscode
```

Remember to always switch back to before committing any changes:
```bash
mv .vscode .vscode-wsl
```