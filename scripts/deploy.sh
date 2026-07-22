#!/usr/bin/env bash
set -euo pipefail

nix flake update --flake ~/.nixos desktop-manager
nix flake update --flake ~/.nixos desktop-manager-cli
echo "Rebuild system now"
