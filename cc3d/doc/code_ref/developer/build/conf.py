import os
import sys
# sys.path.insert(0, os.path.abspath('.'))
import cc3d
from cc3d import CompuCellSetup
from cc3d.cpp import CompuCell

# -- General configuration ------------------------------------------------

# Sphinx extension module names (e.g., 'sphinx.ext.*')
extensions = ['sphinx.ext.mathjax',
              'sphinx.ext.githubpages',
              'sphinx.ext.autodoc',
              'sphinx.ext.autosummary',
              'sphinx.ext.graphviz',
              'sphinx.ext.inheritance_diagram']

# Paths that contain templates here, relative to this directory.
# templates_path = ['_templates']
templates_path = []

# The suffix(es) of source filenames.
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# Make dot accessible; unsure if this is necessary otherwise
graphviz_reldirs = ("Library", "bin") if sys.platform.startswith('win') else ("bin",)
try:
    graphviz_dot = [x for x in os.listdir(os.path.join(os.path.dirname(sys.executable), *graphviz_reldirs))
                    if x.startswith("dot.")][0]
except IndexError:
    raise EnvironmentError('Graphviz not located in environment.')
graphviz_dot = os.path.join(os.path.dirname(sys.executable), "Library", "bin", graphviz_dot)


# General information about the project.
project = u'CC3D_code_developer_reference_manual'
copyright = u'2020, T.J. Sego, Juliano F. Gianlupi, Maciej H. Swat, James A. Glazier'
author = u'T.J. Sego, Juliano F. Gianlupi, Maciej H. Swat, James A. Glazier'
project_title = u'CC3D Code Developer Reference Manual'

# The version info
#   Short X.Y version.
version = cc3d.__version__
#   Full version, including alpha/beta/rc tags.
release = cc3d.__version__

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False

# Auto-generation specification
autosummary_generate = True
autodoc_mock_imports = ["cc3d", "cc3d.CompuCellSetup"]
autoclass_content = "both"
autodoc_default_flags = ["members", "undoc-members", "show-inheritance"]

# Clear problematic stuff
CompuCell.cvar = None

# Filter


def autodoc_skip_member(app, what, name, obj, skip, options):
    """
    from https://het.as.utexas.edu/HET/Software/Sphinx/ext/autodoc.html#confval-autodoc_default_flags
    Emitted when autodoc has to decide whether a member should be included in the documentation.
    The member is excluded if a handler returns True. It is included if the handler returns False.

    :param app: the Sphinx application object
    :param what: the type of the object which the docstring belongs to (one of "module", "class", "exception",
            "function", "method", "attribute")
    :param name: the fully qualified name of the object
    :param obj: the object itself
    :param skip: a boolean indicating if autodoc will skip this member if the user handler does not override the
            decision
    :param options: the options given to the directive: an object with attributes inherited_members, undoc_members,
                show_inheritance and noindex that are true if the flag option of same name was given to the auto
                directive
    :rtype: bool
    """
    if name.endswith("_swigregister"):
        return True
    return False


def setup(app):
    app.connect('autodoc-skip-member', autodoc_skip_member)


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'basic'
# html_theme = 'alabaster'
# html_theme = 'sphinx_rtd_theme'

# on_rtd = os.environ.get('READTHEDOCS', None) == 'True'
#
# if not on_rtd:  # only import and set the theme if we're building docs locally
#     import sphinx_rtd_theme
#     html_theme = 'sphinx_rtd_theme'
#     html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


# -- Options for HTMLHelp output ------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = project


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, project + '.tex', project_title, author, 'howto'),
]


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, project, project_title, [author], 1)
]


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, project, project_title,
     author, project, 'One line description of project.',
     'Miscellaneous'),
]
