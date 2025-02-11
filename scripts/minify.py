import re

# Arquivos de entrada
html_file = "../main/pages/index_home.html"
css_file = "../main/pages/style_home.css"
js_file = "../main/pages/script_home.js"
output_file = "index_home.min.html"

# Lê HTML, CSS e JavaScript
with open(html_file, "r", encoding="utf-8") as f:
    html = f.read()

with open(css_file, "r", encoding="utf-8") as f:
    css = f.read()

with open(js_file, "r", encoding="utf-8") as f:
    js = f.read()
# Minificar CSS: remove comentários e espaços extras
css = re.sub(r"/\*.*?\*/", "", css, flags=re.S)  # Remove comentários de bloco
css = re.sub(r"\s+", " ", css).strip()  # Remove espaços extras

# Minificar JavaScript corretamente:
def minify_js(js_code):
    # Remove comentários de bloco (/* ... */)
    js_code = re.sub(r"/\*.*?\*/", "", js_code, flags=re.S)
    
    # Remove comentários de linha "//", mas **preserva dentro de strings**
    js_code = re.sub(r'(?<!:)//.*', '', js_code)  # Remove apenas se não for parte de um objeto JSON
    
    # Remove espaços e quebras de linha extras
    js_code = re.sub(r"\s+", " ", js_code).strip()

    return js_code

js = minify_js(js)

# Substituir o link do CSS pelo CSS embutido
html = re.sub(r'<link rel="stylesheet" href="style_home.css">', f"<style>{css}</style>", html)

# Substituir o script externo pelo JavaScript embutido
html = re.sub(r'<script src="script_home.js"></script>', f"<script>{js}</script>", html)

# Minificar HTML: remove quebras de linha e espaços extras
html = re.sub(r">\s+<", "><", html)  # Remove espaços entre tags
html = re.sub(r"\s+", " ", html).strip()  # Remove espaços extras

# Salva o HTML minificado
with open(output_file, "w", encoding="utf-8") as f:
    f.write(html)

print(f"Minificação concluída: {output_file}")

