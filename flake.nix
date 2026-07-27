{
  description = "DesktopManager flake with devShell and build derivation";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs }: let
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
      {
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
        };
      }
    );

    # Default package for `nix run .`
    defaultPackage = self.packages.${builtins.currentSystem}.desktop-manager;
  };
}
