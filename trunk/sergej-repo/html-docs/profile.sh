# /etc/profile.d/html-dtd.sh

if [ -f "/usr/share/doc/html/HTML4.cat" ]; then
  SGML_CATALOG_FILES="${SGML_CATALOG_FILES}${SGML_CATALOG_FILES:+:}/usr/share/doc/html/HTML4.cat"
  export SGML_CATALOG_FILES
fi

# /etc/profile.d/html-dtd.sh ends here.
