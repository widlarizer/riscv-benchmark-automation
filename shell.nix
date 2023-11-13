{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell rec {
    # nativeBuildInputs is usually what you want -- tools you need to run
    # nativeBuildInputs = with pkgs.buildPackages; [ pandoc python3Packages.withPackages(ps: with ps; [ pandocfilters ]) ];
  venvDir = "./.venv";
  buildInputs = [
    # A Python interpreter including the 'venv' module is required to bootstrap
    # the environment.
    pkgs.python3Packages.python

    # This execute some shell code to initialize a venv in $venvDir before
    # dropping into the shell
    pkgs.python3Packages.venvShellHook

    pkgs.spike
    pkgs.dtc
 ];

  # Run this command, only after creating the virtual environment
  postVenvCreation = ''
    unset SOURCE_DATE_EPOCH
    pip install -r requirements.txt
  '';

  # Now we can execute any commands within the virtual environment.
  # This is optional and can be left out to run pip manually.
  postShellHook = ''
    # allow pip to install wheels
    unset SOURCE_DATE_EPOCH
    source ./env
  '';
}
