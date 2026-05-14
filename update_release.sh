#!/bin/bash
# update_release.sh - Automate changelog, commit, tag, and push for Smart-Thermostat-Alt-Firmware
# Usage: Run from the Smart-Thermostat-Alt-Firmware workspace after syncing new firmware files.

set -e

# Path to main firmware file containing version string
FIRMWARE_FILE="src/Main-Thermostat.cpp"
CHANGELOG="CHANGELOG.md"

# Extract version from firmware file (expects: const String sw_version = "1.5.003";)
VERSION=$(grep -E 'sw_version\s*=\s*"[0-9]+\.[0-9]+\.[0-9]+"' "$FIRMWARE_FILE" | sed -E 's/.*sw_version\s*=\s*"([0-9]+\.[0-9]+\.[0-9]+)".*/\1/')
if [[ -z "$VERSION" ]]; then
  echo "Error: Could not extract version from $FIRMWARE_FILE."
  exit 1
fi

echo "Detected firmware version: $VERSION"

# Check CHANGELOG for matching version header
if ! grep -q "\[$VERSION\]" "$CHANGELOG"; then
  echo "Warning: $CHANGELOG does not have a header for version $VERSION."
  read -p "Continue anyway? [y/N]: " yn
  case $yn in
    [Yy]*) ;;
    *) echo "Aborting."; exit 1;;
  esac
fi

# Stage all changes
if ! git diff --quiet || ! git diff --cached --quiet; then
  git add -A
  git commit -m "Release $VERSION"
else
  echo "No changes to commit."
fi

# Create tag if it doesn't exist
if git rev-parse "v$VERSION" >/dev/null 2>&1; then
  echo "Tag v$VERSION already exists."
else
  git tag -a "v$VERSION" -m "Release $VERSION"
  echo "Created tag v$VERSION."
fi

echo "To push:"
echo "  git push origin main"
echo "  git push origin v$VERSION"
