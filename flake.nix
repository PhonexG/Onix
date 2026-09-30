{
  description = "Onix - lightweight, high-performance C++20 code editor";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      # Supported architectures
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      
      # Function to generate configuration for each supported system
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell.override {
          # Default shell environment for development
          stdenv = pkgs.gcc14Stdenv; 
        } {
          packages = with pkgs; [
            # Build tools
            cmake
            ninja
            pkg-config

            # Debuger
            gdb

            # supportedSystems
            catch2_3
          ];

          shellHook = ''
            echo "⚡ Onix C++20 dev environment loaded!"
            echo "Compiler: $($CXX --version | head -n 1)"
          '';
        };
      });
    };
}
