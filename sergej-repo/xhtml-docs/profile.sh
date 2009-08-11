# /etc/profile.d/xhtml-dtd.sh

if [ -f "/usr/share/doc/xhtml/DTD/xhtml.soc" ]; then
  SGML_CATALOG_FILES="${SGML_CATALOG_FILES}${SGML_CATALOG_FILES:+:}/usr/share/doc/xhtml/DTD/xhtml.soc"
  export SGML_CATALOG_FILES
fi

# /etc/profile.d/xhtml-dtd.sh ends here.
