#include <fc/lib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    fc_dom_t *dom = fc_dom_new();
    fc_term_init();
    fc_term_altscreen(true);
    fc_term_cursor(false);

    int W = dom->w, H = dom->h;

    fc_node_t *main_box;
    {
        main_box = fc_box(dom, 0, 0, W, H);
        fc_box_title(main_box, " box title works i think? ");
        fc_box_title_align(main_box, FC_ALIGN_CENTER); // otherwise its rightbinded
        fc_box_border(main_box, FC_BORDER_DOUBLE);
        fc_box_colors(main_box, FC_CYAN, FC_DEFAULT);
        fc_box_title_color(main_box, FC_BRIGHT_RED);
    }

    fc_node_t *info_txt;
    {
        fc_node_t *info_box = fc_box(dom, 2, 2, 35, 8);
        fc_box_title(info_box, " hello ");
        fc_box_border(info_box, FC_BORDER_ROUNDED);
        fc_box_colors(info_box, FC_YELLOW, FC_DEFAULT);
        fc_node_attach(main_box, info_box);

        info_txt = fc_text(dom, 4, 4, 31, 4);
        fc_text_set(info_txt, "text 1:\ntext 2:");
        fc_text_colors(info_txt, FC_WHITE, FC_DEFAULT);
        fc_node_attach(info_box, info_txt);
    }

    //fc_node_t *bar_cpu;
    {
        //fc_node_t *stats_box = fc_box(dom, 2, 11, 35, 8);

    }

    fc_dom_render(dom);
    sleep(2);

    sleep(3);

    fc_term_cursor(true); // otherwise you dont have a cursor :(
    fc_term_altscreen(false);
    //fc_term_flush();
    //fc_term_shutdown();
    //fc_dom_free(dom);
    //return 0;
}
