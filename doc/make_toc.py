import sys
import re

sys.stdout.reconfigure(errors='ignore')

def slugify(text):
    """Generates a GitHub-compatible anchor slug."""
    # Lowercase the text
    slug = text.lower()
    # Remove all characters that are NOT alphanumeric, whitespace, hyphens, or underscores
    slug = re.sub(r'[^\w\s-]', '', slug)
    # Replace whitespace (spaces, tabs, etc.) with a single hyphen
    slug = re.sub(r'\s+', '-', slug)
    # Remove leading and trailing hyphens
    return slug.strip('-')

def main():
    if len(sys.argv) <= 1:
        print(f"Usage: python {sys.argv[0]} <markdown_file>")
        return

    # User input for depth
    try:
        depth_input = input("How many levels? (1 to 4, default 3): ")
        depth = int(depth_input) if depth_input.strip() else 3
    except ValueError:
        depth = 3

    print("\n## Table of Contents\n")

    try:
        with open(sys.argv[1], 'r', encoding="utf8") as f:
            for line in f:
                # Ignore lines that aren't headers or are the TOC header itself
                if not line.startswith('#') or 'table of content' in line.lower():
                    continue

                # Parse header level and text
                match = re.match(r'^(#+)\s+(.*)', line)
                if not match:
                    continue
                
                hashes, title = match.groups()
                level = len(hashes)

                # Skip H1 (#) and levels deeper than user choice
                if level == 1 or level > depth:
                    continue

                # Generate slug and format list item
                # level-2 ensures that ## starts with 0 indentation
                indent = '    ' * (level - 2)
                anchor = slugify(title)
                print(f"{indent}- [{title.strip()}](#{anchor})")
                
    except FileNotFoundError:
        print(f"Error: File '{sys.argv[1]}' not found.")

if __name__ == "__main__":
    main()