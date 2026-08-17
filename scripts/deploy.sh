#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

git -C "$PROJECT_DIR" pull
nix flake update --flake "$PROJECT_DIR"
git -C "$PROJECT_DIR" add flake.lock
git -C "$PROJECT_DIR" commit --amend --no-edit
git -C "$PROJECT_DIR" push --force-with-lease

nix flake update --flake ~/.nixos desktop-manager
nix flake update --flake ~/.nixos desktop-manager-cli
echo "Rebuild system now"
