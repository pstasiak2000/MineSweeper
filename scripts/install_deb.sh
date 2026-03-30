#!/usr/bin/env bash
set -e

REPO="pstasiak2000/MineSweeper"
API_URL="https://api.github.com/repos/$REPO/releases/latest"

echo "🔍 Fetching latest release..."

DEB_URL=$(curl -s "$API_URL" \
  | grep browser_download_url \
  | grep amd64.deb \
  | cut -d '"' -f 4)

if [ -z "$DEB_URL" ]; then
    echo "❌ Failed to find .deb file"
    exit 1
fi

TMP_FILE=$(mktemp --suffix=.deb)

echo "⬇️ Downloading package..."
curl -L "$DEB_URL" -o "$TMP_FILE"

echo "DEB URL: $DEB_URL"
echo "TMP FILE: $TMP_FILE"

echo "📦 Installing..."
sudo apt install -y "$TMP_FILE"

echo "🧹 Cleaning up..."
rm "$TMP_FILE"

echo "✅ Installation complete!"
