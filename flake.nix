{
  description = "DesktopManager flake with devShell and build derivation";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    logging-src = {
      url = "github:arabbitplays/Logging-Library";
      flake = false;
    };
    terminal-renderer-src = {
      url = "github:arabbitplays/Terminal-Renderer-Library";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, logging-src, terminal-renderer-src }: let
    systems = [ "x86_64-linux" "aarch64-linux" ];
    forAllSystems = nixpkgs.lib.genAttrs systems;
  in
  {
    # Development shells
    devShells = forAllSystems (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        default = pkgs.mkShell {
          packages = with pkgs; [];

          buildInputs = with pkgs; [
            meson
            ninja
            pkg-config
            python3
          ];

          shellHook = ''
            export SHELL=${pkgs.zsh}/bin/zsh
            echo "Entered DesktopManager dev environment for ${system}"
            exec ${pkgs.zsh}/bin/zsh
          '';
        };
      }
    );

    # Packages / build derivation
    packages = forAllSystems (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      rec {
        desktop-manager = pkgs.stdenv.mkDerivation {
          pname = "DesktopManager";
          version = "1.0.0";

          src = self;

          nativeBuildInputs = with pkgs; [
            meson
            ninja
            pkg-config
            python3
          ];

          # Pull the subproject sources from flake inputs and drop them into
          # subprojects/ so meson finds them locally and skips the wrap fetch.
          postPatch = ''
            cp -r --no-preserve=mode,ownership ${logging-src} subprojects/logging
            cp -r --no-preserve=mode,ownership ${terminal-renderer-src} subprojects/terminal_renderer
          '';
        };

        default = desktop-manager;
      }
    );
  };
}
