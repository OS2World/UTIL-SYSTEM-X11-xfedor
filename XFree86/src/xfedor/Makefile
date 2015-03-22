LIBS= -L/XFree86/lib/ -lX11 -lXpm -lm

SOSDIR=/XFree86/lib/X11/xfedor

CC=gcc
CFLAGS= -I. -I/XFree86/include -I/XFree86/include/X11 -O2 -DSOSDIR=\"$(SOSDIR)\"
XFEDOR= couchex.o myrast.o dialogue.o popup.o tr_tabbord.o tr_garb.o tr_num.o tr_save.o tr_font.o tr_edit.o tr_extra.o tr_test.o tr_grille.o tr_colormap.o otheredit.o otherfont.o filer.o bitmap.o rubber.o sos.o ed_point.o ed_line.o ed_rect.o ed_cercle.o ed_write.o ed_metric.o ed_cutap.o ed_resize.o ed_fill.o ed_transl.o ed_rotate.o ed_beziers.o main.o

xfedor.exe: $(XFEDOR)
	gcc -o xfedor.exe $(XFEDOR) $(LIBS)
