PHP_ARG_ENABLE(splclassloader, whether to enable splclassloader,
[ --enable-splclassloader	Enable splclassloader support])

if test "$PHP_SPLCLASSLOADER" != "no"; then

  PHP_NEW_EXTENSION(splclassloader, splclassloader.c, $ext_shared)

fi
