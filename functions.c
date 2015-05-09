#include "functions.h"

void keyboard() {
    flash_screen();
    display_kenet();
    while(!esc) {
        test_keyboard();
        record();
        init_coords();
        display_text(color_car, color_omb, SDL_ALPHA_OPAQUE);
        display_cursor();
        if(delay) {
            SDL_Delay(delay);
            delay=0;
       }
       new_screen();
    }
}

void new_screen() {
    int nb=10, i;
    if(last_chars[0]=='c'&&last_chars[1]=='l'&&last_chars[2]=='s') {
        double r=(double)RED; double g=(double)GREEN; double b=(double)BLUE;
        double st_r=((double)255-r)/(double)nb;
        double st_g=((double)255-g)/(double)nb;
        double st_b=((double)255-b)/(double)nb;

        for(i=0; i<nb; i++) {
            color1.r=(int)r;
            color1.g=(int)g;
            color1.b=(int)b;
            typing=1;
            display_text(color1, color_omb, SDL_ALPHA_OPAQUE);
            r+=st_r; g+=st_g; b+=st_b;
        }
        last_chars[0]=' ';
        last_chars[1]=' ';
        last_chars[2]=' ';
        for(i=SDL_ALPHA_OPAQUE; i>=SDL_ALPHA_TRANSPARENT; i-=20) {
            typing=1;
            display_text(color1, color_omb, i);
        }
        //
        for(i=0; i<nb_lines; i++) {
            array_car[i*200]='\0';
        }
        ind_car_x=0;
        ind_car_y=0;
    }
}

void record() {
    if(typing) {
        if(tick!=0) {
            wait_val=SDL_GetTicks()-tick;
            printf("wait=%d\n", wait_val);
            wait_val&=0x3fffffff;
            wait_val|=0x40000000;
            array_record[index_record++]=wait_val;
        }
        tick=SDL_GetTicks();
        //
        if(car_ok) {
            array_record[index_record++]=car_ok|0x20000000;
        } else {
            array_record[index_record++]=actual_char;
            last_chars[0]=last_chars[1];
            last_chars[1]=last_chars[2];
            last_chars[2]=actual_char;
        }
    }
}

void display_text(SDL_Color color1, SDL_Color color2, int alpha) {
    if(typing) {
        typing=0;
        SDL_BlitSurface(p_kenet, NULL, p_fond, NULL);
        int i, w, h;
        int x_pos=10, y_pos=10;
        SDL_Surface *text_surface;
        SDL_Surface *text_surface_shade;
        SDL_Rect rect;
        for(i=0; i<nb_lines; i++) {
            TTF_SizeText(font, (const char*)array_car+200*i, &w, &h);
            text_surface_shade=TTF_RenderText_Solid(font, (const char*)array_car+200*i, color2);
            rect.x=x_pos+2;
            rect.y=y_pos+2;
            rect.w=w;
            rect.h=h;
            SDL_SetSurfaceAlphaMod(text_surface_shade, alpha);
            SDL_BlitSurface(text_surface_shade, NULL, p_fond, &rect);
            SDL_FreeSurface(text_surface_shade);
            //
            text_surface=TTF_RenderText_Solid(font, (const char*)array_car+200*i, color1);
            rect.x=x_pos;
            rect.y=y_pos;
            rect.w=w;
            rect.h=h;
            SDL_SetSurfaceAlphaMod(text_surface, alpha);
            SDL_BlitSurface(text_surface, NULL, p_fond, &rect);
            SDL_FreeSurface(text_surface);
            y_pos=y_pos+h+5;
        }
        p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond);
        SDL_RenderClear(p_renderer);
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        SDL_DestroyTexture(p_texture);
   }
}

void init_coords() {
    int w, h;
    //
    x=10; y=10;
    int i;
    for(i=0; i<ind_car_x; i++) {
        line_char[i]=array_car[ind_car_y*200+i];
    }
    line_char[i]='\0';
    TTF_SizeText(font, (const char*)line_char, &w, &h);
    x+=w;
    //
    for(i=0; i<ind_car_y; i++) {
        TTF_SizeText(font, (const char*)array_car+i*200, &w, &h);
        y=y+h+5;
    }
    //printf("x=%d y=%d\n", x, y);
}

void test_keyboard() {
    int writting=0;
    car_ok=0;
    //
    SDL_Scancode key_scancode;
    SDL_Keycode key_code;
    SDL_Keymod key_mod=SDL_GetModState();
    upper=0;
    typing=0;
    if(key_mod&KMOD_SHIFT) {
        upper=1;
    }
    SDL_PollEvent(&event);
      switch(event.type) {
        case SDL_KEYDOWN:
            key_scancode=event.key.keysym.scancode;
            switch(key_scancode) {
                case SDL_SCANCODE_ESCAPE:
                esc=1;
                break;
                /*case SDL_SCANCODE_RSHIFT|SDL_SCANCODE_LSHIFT:
                    upper=1;
                    break;*/
                case SDL_SCANCODE_SPACE:
                    car_ok=1;
                    typing=1;
                break;
                case SDL_SCANCODE_BACKSPACE:
                    delay=50;
                    car_ok=2;
                    typing=1;
                    break;
                case SDL_SCANCODE_DELETE:
                    delay=50;
                    car_ok=6;
                    typing=1;
                    break;
                case SDL_SCANCODE_UP:
                        car_ok=3;
                        typing=1;
                        delay=50;
                    break;
                case SDL_SCANCODE_LEFT:
                    delay=50;
                    car_ok=4;
                    typing=1;
                    break;
                case SDL_SCANCODE_RIGHT:
                    delay=50;
                    car_ok=5;
                    typing=1;
                     break;
                case SDL_SCANCODE_DOWN:
                    delay=50;
                    car_ok=8;
                    typing=1;
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                    delay=200;
                    car_ok=7;
                    typing=1;
                    break;
                case SDL_SCANCODE_KP_0:
                case SDL_SCANCODE_KP_1:
                case SDL_SCANCODE_KP_2:
                case SDL_SCANCODE_KP_3:
                case SDL_SCANCODE_KP_4:
                case SDL_SCANCODE_KP_5:
                case SDL_SCANCODE_KP_6:
                case SDL_SCANCODE_KP_7:
                case SDL_SCANCODE_KP_8:
                case SDL_SCANCODE_KP_9:
                    typing=1;
                    car_ok=9;
                    break;
                case SDL_SCANCODE_KP_PERIOD:
                    car_ok=10;
                    typing=1;
                    break;
                default:
                    key_code=SDL_GetKeyFromScancode(key_scancode);
                    car=SDL_GetKeyName(key_code);
                    typing=1;
                 break;
             }
            break;
            /*case SDL_KEYUP:
               case SDL_SCANCODE_RSHIFT|SDL_SCANCODE_LSHIFT:
                    upper=0;
                break;
             break;*/
     }
        if(typing) {
                   if(car_ok||strlen(car)==1) {
                   if(car_ok) {
                         switch(car_ok) {
                            case 1:
                                actual_char=' ';
                                writting=1;
                                break;
                            case 2:
                                key_backspace();
                                break;
                            case 3:
                                key_up();
                                break;
                            case 4:
                                ind_car_x--;
                                if(ind_car_x<0) {
                                    typing=0;
                                    ind_car_x=0;
                                }
                            break;
                            case 5:
                                if(array_car[ind_car_y*200+ind_car_x]!='\0') {
                                    ind_car_x++;
                                } else {
                                    typing=0;
                                }
                                break;
                            case 6:
                                if(array_car[ind_car_y*200+ind_car_x]!='\0') {
                                    int ind_car=ind_car_x;
                                    while(array_car[ind_car_y*200+ind_car]!='\0') {
                                        array_car[ind_car_y*200+ind_car]=array_car[ind_car_y*200+ind_car+1];
                                        ind_car++;
                                    }
                                } else {
                                    typing=0;
                                }
                                break;
                            case 7: //enter
                                make_enter();
                                break;
                            case 8:
                                key_down();
                                break;
                            case 9:
                                switch(key_scancode) {
                                    case SDL_SCANCODE_KP_0:
                                        actual_char='0';
                                        break;
                                    case SDL_SCANCODE_KP_1:
                                        actual_char='1';
                                        break;
                                    case SDL_SCANCODE_KP_2:
                                        actual_char='2';
                                        break;
                                    case SDL_SCANCODE_KP_3:
                                        actual_char='3';
                                        break;
                                    case SDL_SCANCODE_KP_4:
                                        actual_char='4';
                                        break;
                                    case SDL_SCANCODE_KP_5:
                                        actual_char='5';
                                        break;
                                    case SDL_SCANCODE_KP_6:
                                        actual_char='6';
                                        break;
                                    case SDL_SCANCODE_KP_7:
                                        actual_char='7';
                                        break;
                                    case SDL_SCANCODE_KP_8:
                                        actual_char='8';
                                        break;
                                    case SDL_SCANCODE_KP_9:
                                        actual_char='9';
                                        break;
                                }
                                writting=1;
                                break;
                                case 10:
                                    actual_char='.';
                                    writting=1;
                                    break;
                        }
                    } else {
                        actual_char=*car;
                        TTF_SizeText(font, car, &width, &height);
                        if(width>0) {
                         if(upper&&(actual_char>=97&&actual_char<=122)) {
                            actual_char-=32;
                        } else if(!upper&&(actual_char>=65&&actual_char<=90)) {
                            actual_char+=32;
                        }
                        writting=1;
                    } else {
                        typing=0;
                    }
                }
                } else {
                    typing=0;
                }
            }
            if(typing&&writting) {
                record_char(actual_char);
            }

}

void record_char(char c_char) {
    if(car_ok==1||car_ok==9||car_ok==10) {
        car_ok=0;
    }
    strcpy(line_char, array_car+ind_car_y*200);
    if(line_char[ind_car_x]=='\0') {
        line_char[ind_car_x]=c_char;
        line_char[ind_car_x+1]='\0';
    } else {
        strcpy(line_char_2, line_char+ind_car_x);
        line_char[ind_car_x]=c_char;
        line_char[ind_car_x+1]='\0';
        strcat(line_char, line_char_2);
    }

    if(widthOK()) {
        strcpy(array_car+ind_car_y*200, line_char);
        ind_car_x++;
    } else {
        typing=0;
    }
}

void key_down() {
    if(ind_car_y<(nb_lines-1)) {
        ind_car_y++;
        int ind_x=0;
        while((array_car[ind_car_y*200+ind_x]!='\0')&&ind_x<ind_car_x) {
            ind_x++;
        }
        ind_car_x=ind_x;
    } else {
        typing=0;
    }
}

void key_backspace() {
    if(array_car[ind_car_y*200+ind_car_x]=='\0') {
        if(ind_car_x>0) {
            ind_car_x--;
            array_car[ind_car_y*200+ind_car_x]='\0';
        } else {
            typing=0;
        }
    } else {
        if(ind_car_x>0) {
            strcpy(line_char, array_car+ind_car_y*200+ind_car_x);
            ind_car_x--;
            array_car[ind_car_y*200+ind_car_x]='\0';
            strcat(array_car+ind_car_y*200, line_char);
        } else {
            typing=0;
        }
    }
}

void make_enter() {
    int i;
    if(array_car[ind_car_y*200+ind_car_x]=='\0') {
        if(ind_car_y==(nb_lines-1)) {
            for(i=0; i<nb_lines-2; i++) {
                strcpy(array_car+i*200, array_car+(i+1)*200);
            }
            array_car[(nb_lines-1)*200]='\0';
            ind_car_x=0;
        } else {
            for(i=nb_lines-2; i>=(ind_car_y+1); i--) {
                strcpy(array_car+200*(i+1), array_car+200*i);
            }
            ind_car_y++;
            ind_car_x=0;
            array_car[200*ind_car_y]='\0';
        }
    } else {
        if(ind_car_y==nb_lines-1) {
            strcpy(line_char, array_car+ind_car_y*200);
            line_char[ind_car_x]='\0';
            strcpy(line_char_2, array_car+ind_car_y*200+ind_car_x);
            //
            for(i=0; i<nb_lines-2; i++) {
                strcpy(array_car+i*200, array_car+(i+1)*200);
            }
            strcpy(array_car+(nb_lines-2)*200, line_char);
            strcpy(array_car+(nb_lines-1)*200, line_char_2);
            ind_car_x=0;
        }
    }
}

void key_up() {
    if(ind_car_y>0) {
        ind_car_y--;
        int ind_x=0;
        char ind_car;
        do {
           ind_car=array_car[ind_car_y*200+ind_x++];
        } while(ind_car!='\0'&&ind_x<ind_car_x);
        ind_x--;
        ind_car_x=ind_x;
        } else {
            typing=0;
        }
}

int widthOK() {
    int w, h;
    TTF_SizeText(font, (const char*)array_car+200*ind_car_y, &w, &h);
    if(w<(980)) {
        return 1;
    }
    return 0;
}

void display_cursor() {
 //
    index_curv_curs++;
    if(index_curv_curs>=CURVCURS) {
        index_curv_curs=0;
    }
    SDL_Color color={array_curv_curs[index_curv_curs][0], array_curv_curs[index_curv_curs][1],array_curv_curs[index_curv_curs][2]};
    p_cursor=TTF_RenderText_Solid(font, "_", color);
    SDL_Rect omb1={x-1, y+5-1, w_cursor, h_cursor};
    SDL_BlitSurface(p_cursor_2, NULL, p_fond, &omb1);
    SDL_Rect omb2={x+1, y+5+1, w_cursor, h_cursor};
    SDL_BlitSurface(p_cursor_2, NULL, p_fond, &omb2);
    SDL_Rect rect={x, y+5, w_cursor, h_cursor};
    SDL_BlitSurface(p_cursor, NULL, p_fond, &rect);
    SDL_FreeSurface(p_cursor);
    p_cursor=NULL;
    p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond);
    SDL_RenderClear(p_renderer);
    SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
    SDL_RenderPresent(p_renderer);
    SDL_DestroyTexture(p_texture);
 }

void display_kenet() {
    int alpha=255;
    int loop=1;
    while(loop) {
       if(alpha<0) {
            loop=0;
            alpha=0;
        }
        SDL_SetTextureAlphaMod(p_texture_fond_blanc, alpha);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_kenet, NULL, NULL);
        SDL_RenderCopy(p_renderer, p_texture_fond_blanc, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        alpha-=10;
    }
    SDL_RenderClear( p_renderer );
    SDL_RenderCopy(p_renderer, p_texture_kenet, NULL, NULL);
    SDL_RenderPresent(p_renderer);
    SDL_BlitSurface(p_kenet, NULL, p_fond, NULL);
}

void flash_screen() {
    int alpha=0;
    int loop=1;
    while(loop) {
        if(alpha>255) {
            loop=0;
            alpha=255;
        }
        SDL_SetTextureAlphaMod(p_texture_fond_blanc, alpha);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_fond_blanc, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        alpha+=15;
    }
}

void init() {
    pixel_format=SDL_PIXELFORMAT_ARGB8888;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS|SDL_INIT_TIMER);
    p_window=SDL_CreateWindow("HMD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_BORDERLESS);
    IMG_Init(IMG_INIT_PNG);
    p_kenet=IMG_Load("gfx/darko.png");
    p_fond_blanc=IMG_Load("gfx/fondblanc.png");
    p_renderer=SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
     //
    SDL_SetRenderDrawColor( p_renderer, 0, 0, 0, 0 );
    SDL_RenderClear( p_renderer );
    //
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    p_fond=SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, rmask, gmask, bmask, amask);
    p_fond_blanc=SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(p_fond_blanc, NULL, SDL_MapRGB(p_fond_blanc->format, 255, 255, 255));
    p_texture_kenet=SDL_CreateTextureFromSurface(p_renderer, p_kenet);
    p_texture_fond_blanc=SDL_CreateTextureFromSurface(p_renderer, p_fond_blanc);
    //
    //
    TTF_Init();
    font=TTF_OpenFont("fonts/Computerfont.ttf", 32);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    max_height=TTF_FontHeight(font);
    //
    nb_lines=(768-20)/(max_height+5);
    array_car=malloc(nb_lines*200*sizeof(char));
    int i;
    for(i=0; i<nb_lines; i++) {
        array_car[i*200]='\0';
    }
    new_char=malloc(2*sizeof(char));
    car_ok=0; delay=0;
    line_char=malloc(200*sizeof(char));
    line_char_2=malloc(200*sizeof(char));
    ind_car_x=0; ind_car_y=0;
    last_chars[0]=' ';
    last_chars[1]=' ';
    last_chars[2]=' ';
    //
    color_omb.r=RED2; color_omb.g=GREEN2; color_omb.b=BLUE2;
    color_car.r=RED; color_car.g=GREEN; color_car.b=BLUE;
    p_cursor_2=TTF_RenderText_Blended(font, "_", color_omb);
    w_cursor=p_cursor_2->w; h_cursor=p_cursor_2->h;
     //
    esc=0; x=10; last_x=10; y=10;
    typing=0; upper=0; last_index=0;
    //
    int red=RED, green=GREEN, blue=BLUE;
    double r=(double)red/(double)2, g=(double)green/(double)2, b=(double)blue/(double)2;
    double angle=0.0;
    double step=(double)2*M_PI/(double)CURVCURS;
    for(i=0; i<CURVCURS; i++) {
        array_curv_curs[i][0]=(int)floor(r*cos(angle)+r+0.5);
        array_curv_curs[i][1]=(int)floor(g*cos(angle)+g+0.5);
        array_curv_curs[i][2]=(int)floor(b*cos(angle)+b+0.5);
        angle+=step;
    }
    index_curv_curs=0;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //
    tick=0; wait_val=0;
    array_record=malloc(sizeof(int)*NB_RECORDS);
    index_record=0;
}


void quit() {
    TTF_CloseFont(font);
    font=NULL;
    TTF_Quit();
    SDL_FreeSurface(p_cursor_2);
    SDL_FreeSurface(p_kenet);
    SDL_FreeSurface(p_fond_blanc);
    SDL_FreeSurface(p_fond);
    SDL_DestroyTexture(p_texture_fond_blanc);
    SDL_DestroyTexture(p_texture_kenet);
    SDL_DestroyTexture(p_texture_cursor_2);
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyWindow(p_window);
    IMG_Quit();
    SDL_Quit();
    free(line_char);
    free(line_char_2);
    free(array_car);
    free(new_char);
    save_record();
    free(array_record);
}

void save_record() {
    if(index_record>NB_RECORDS) {
        printf("Allocation trop faible : index=%d\n", index_record);
    } else {
        fichier=fopen("keys", "w");
        fprintf(fichier, "int size=%d;\n", index_record);
        fprintf(fichier, "{ ");
        int i, j=0;
        for(i=0; i<index_record; i++) {
            fprintf(fichier, "%u, ", array_record[i]);
            j++;
            if(j>=30) {
                j=0;
                fprintf(fichier, "\n");
            }
        }
        fclose(fichier);
        printf("nombre d'éléments=%d\n", index_record);
    }
}
