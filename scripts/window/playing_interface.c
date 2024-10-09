#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Inclure SDL_ttf pour le rendu du texte
#include <stdio.h>
#include <string.h>
#include "../../headers/window/options_play.h"
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/player.h"
#include "../../headers/logics.h"
#include "../../headers/window/place_boat.h"
#include "../../headers/graphics.h"
#include "../../headers/damier.h"
#include "../../headers/computer_mode.h"
#include "../../headers/ai.h"
#define MAX_SHOTS 4
// Variables pour la gestion des tirs
int selected_cells[MAX_SHOTS][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
int num_shots = 0;
void DrawSelectedCells(Grid* grid, int selected_cells[MAX_SHOTS][2], int num_shots, SDL_Texture* targetTexture) {
    // Dessiner chaque cellule sélectionnée
    for (int i = 0; i < num_shots; i++) {
        SDL_Rect selectedRect = {grid->xPos + selected_cells[i][0] * grid->cellSize, 
                                 grid->yPos + selected_cells[i][1] * grid->cellSize, 
                                 grid->cellSize, grid->cellSize};

        // Assurez-vous que l'alpha est réinitialisé pour être complètement opaque
        SDL_SetTextureAlphaMod(targetTexture, 255);
        SDL_RenderCopy(grid->renderer, targetTexture, NULL, &selectedRect);
    }
}

int DrawHints(Grid* grid, int hint[10][10]) {
    // Limitez les itérations à la taille minimale entre la grille et la matrice de hints
    int maxWidth = (grid->gridWidth < 10) ? grid->gridWidth : 10;
    int maxHeight = (grid->gridHeight < 10) ? grid->gridHeight : 10;

    for (int j = 0; j < maxHeight; j++) { // Itération sur les lignes (axe Y)
        for (int i = 0; i < maxWidth; i++) { // Itération sur les colonnes (axe X)
            SDL_Rect cellRect = {
                grid->xPos + i * grid->cellSize,
                grid->yPos + j * grid->cellSize,
                grid->cellSize,
                grid->cellSize
            };

            if (hint[j][i] == 2) { // Accès corrigé : hint[y][x]
                SDL_SetRenderDrawColor(grid->renderer, 255, 0, 0, 255);  // Rouge pour un touché
                SDL_RenderFillRect(grid->renderer, &cellRect);
            } else if (hint[j][i] == 1) { // Accès corrigé : hint[y][x]
                SDL_SetRenderDrawColor(grid->renderer, 0, 0, 255, 255);  // Bleu pour un raté
                SDL_RenderFillRect(grid->renderer, &cellRect);
            }
            else if(hint[j][i]== 3){
                SDL_SetRenderDrawColor(grid->renderer, 0, 0, 0, 255);  // Noir pour une case non touchée
                SDL_RenderFillRect(grid->renderer, &cellRect);
            }
            // Optionnel : Réinitialiser la couleur après le dessin
            SDL_SetRenderDrawColor(grid->renderer, 255, 255, 255, 255); // Blanc par défaut
        }
    }
}

void FireAtCell(Player* adversary, int boat_pos[10][10], int hint[10][10], int col, int row)
{
    printf("Lancement de FireAtCell.\n");
    
    // Validation des coordonnées
    if (col < 0 || col >= 10 || row < 0 || row >= 10) {
        printf("Coordonnées (%d, %d) hors limites!\n", col, row);
        return;
    }

    printf("Tir dans la cellule (Ligne: %d, Colonne: %d)\n", row, col);

    // Vérifier si la cellule a déjà été tirée
    if (hint[row][col] != 0) {
        printf("Cellule déjà tirée\n");
        return; // Si déjà tirée, on ne fait rien
    }

    // Vérifier si un bateau se trouve dans la cellule ciblée
    if (boat_pos[row][col] != 0) {
        // Touché
        adversary->health -= 1;
        printf("Touché!\n");
        printf("Le joueur %s a maintenant %d point(s) de vie.\n", adversary->name, adversary->health);
        int boat_id = boat_pos[row][col];  // Identifier le bateau
        hint[row][col] = 2;  // Marquer comme touché dans la matrice hint

        // Vérifier si la totalité du bateau est coulée
        bool boat_sunk = true;

        // Parcourir l'ensemble de la grille pour vérifier les autres parties du bateau
        for (int r = 0; r < 10 && boat_sunk; r++) {
            for (int c = 0; c < 10; c++) {
                // Si une autre partie du bateau est encore intacte (non touchée), le bateau n'est pas coulé
                if (boat_pos[r][c] == boat_id && hint[r][c] != 2) {
                    boat_sunk = false;
                    break;
                }
            }
        }

        // Si le bateau est coulé, marquer toutes ses parties comme coulées dans hint
        if (boat_sunk) {
            printf("Bateau coulé!\n");
            for (int r = 0; r < 10; r++) {
                for (int c = 0; c < 10; c++) {
                    if (boat_pos[r][c] == boat_id) {
                        hint[r][c] = 3;  // Marquer comme coulé
                    }
                }
            }
        }

    } else {
        // Manqué
        printf("Manqué!\n");
        hint[row][col] = 1;  // Marquer comme manqué dans la matrice hint
    }

    // Afficher les matrices mises à jour
    printf("Grille des bateaux :\n");
    afficherMatrice(boat_pos);
    printf("Grille des tirs :\n");
    afficherMatrice(hint);
}
void FireCallback(){
    printf("fire on a cell");
    // Obtenir les cellules sélectionnées de l'interface utilisateur faire une boucle pour shooter dans les cell
    for (int i = 0; i < num_shots; i++) {
        FireAtCell(&player_two,player_two_grid,hint_player_one_grid, selected_cells[i][0], selected_cells[i][1]);
    }
    // Réinitialiser les cellules sélectionnées
    num_shots = 0;
    //et les selected cells
    for (int i = 0; i < MAX_SHOTS; i++) {
        selected_cells[i][0] = -1;
        selected_cells[i][1] = -1;
    }
    //change who_Played
    //*who_Played=2;
    //faire jouer le joueur 2
    printf("player 2 plying:");
    player_two_acting();
}
void PlayingInterface(SDL_Window* Window, SDL_Renderer* Renderer) {
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    //SDL_RenderClear(Renderer);
      int width = 0; 
    int height = 0; 
    SDL_Color textColor = {255, 255, 255, 255};
    // Charger et afficher l'image de fond
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/metalic_panel.png");
    if (!backgroundSurface) {
        return;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(Renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);

    Fleet fleet = player_one.fleet;

    // Initialisation des grilles
    Grid play_grid;
    play_grid.renderer = Renderer;
    play_grid.gridWidth = 10;
    play_grid.gridHeight = 10;
    play_grid.cellSize = 41;
    play_grid.xPos = 50;
    play_grid.yPos = 90;

    Grid radar_grid;
    radar_grid.renderer = Renderer;
    radar_grid.gridWidth = 10;
    radar_grid.gridHeight = 10;
    radar_grid.cellSize = 31;
    radar_grid.xPos = 530;
    radar_grid.yPos = 230;

    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    radar_grid.backgroundImage = SDL_CreateTextureFromSurface(Renderer, bgSurface);
    play_grid.backgroundImage = SDL_CreateTextureFromSurface(Renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Dessiner les grilles
    DrawGrid(&play_grid);
    DrawGrid(&radar_grid);
    // Ajouter les textures des bateaux
    add_texture_ship(&fleet.aircraft_carrier, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.cruiser, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.destroyer, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.submarine, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    add_texture_ship(&fleet.torpedo_boat, Renderer, "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png");
    //Fire button 

    DrawFleet(&radar_grid, &fleet);
    // Charger l'image de "target"
    SDL_Surface *targetSurface = IMG_Load("medias/images/targeting.png");
    SDL_Texture *targetTexture = SDL_CreateTextureFromSurface(Renderer, targetSurface);
    SDL_FreeSurface(targetSurface);

    

    int quit = 0;
    SDL_Event event;
    int mouse_x, mouse_y;
    int cell_x = -1, cell_y = -1;
    int highlight_x = -1, highlight_y = -1;
    ClearEvents();
     CreateClickableElement(Renderer,200,500,&width,&height, "FIRE!!!", textColor,"medias/images/btn-play.png",FireCallback,12);
    //SDL_RenderPresent(Renderer);
    while (!quit || ((player_one.health!=0)||(player_two.health!=0))) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    // Si la souris est dans la zone de la grille play_grid
                    if (mouse_x >= play_grid.xPos && mouse_x < play_grid.xPos + play_grid.gridWidth * play_grid.cellSize &&
                        mouse_y >= play_grid.yPos && mouse_y < play_grid.yPos + play_grid.gridHeight * play_grid.cellSize) {
                        // Calculer la cellule de la grille
                        cell_x = (mouse_x - play_grid.xPos) / play_grid.cellSize;
                        cell_y = (mouse_y - play_grid.yPos) / play_grid.cellSize;
                        highlight_x = cell_x;
                        highlight_y = cell_y;
                    } else {
                        highlight_x = -1;
                        highlight_y = -1;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    printf("Click handle");
                    TriggerClickCallbacks(mouse_x,mouse_y);
                    printf("hightlight %d",highlight_y);
                    printf("hightlight %d",highlight_x);
                    if(event.button.button == SDL_BUTTON_RIGHT){
                        printf("right click");
                    }
                    if (highlight_x != -1 && highlight_y != -1) {
                        int already_selected = 0;
                        printf("selectionner cellule");
                        // Vérifier si cette cellule est déjà sélectionnée
                        for (int i = 0; i < num_shots; i++) {
                            if (selected_cells[i][0] == highlight_x && selected_cells[i][1] == highlight_y) {
                                already_selected = 1;

                                // Si déjà sélectionné, désélectionner
                                for (int j = i; j < num_shots - 1; j++) {
                                    selected_cells[j][0] = selected_cells[j + 1][0];
                                    selected_cells[j][1] = selected_cells[j + 1][1];
                                }
                                selected_cells[num_shots - 1][0] = -1;
                                selected_cells[num_shots - 1][1] = -1;
                                num_shots--;
                                break;
                            }
                        }

                        // Si non sélectionné et qu'il reste des tirs, ajouter la cellule
                        if (!already_selected && num_shots < MAX_SHOTS) {
                            printf("selected!!");
                            selected_cells[num_shots][0] = highlight_x;
                            selected_cells[num_shots][1] = highlight_y;
                            num_shots++;
                        }
                    }
                    break;
            }
        }

        // Redessiner l'interface
        //SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        //SDL_RenderClear(Renderer);

        // Redessiner les éléments de la grille
        //SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);
        DrawGrid(&play_grid);
        DrawGrid(&radar_grid);
        DrawFleet(&radar_grid,&fleet);
        DrawHints(&play_grid,hint_player_one_grid);
        DrawHints(&radar_grid,hint_player_two_grid);
        // Dessiner la "target" qui suit la souris dans la grille
        if (highlight_x != -1 && highlight_y != -1) {
            SDL_Rect targetRect = {play_grid.xPos + highlight_x * play_grid.cellSize, 
                                   play_grid.yPos + highlight_y * play_grid.cellSize, 
                                   play_grid.cellSize, play_grid.cellSize};
            SDL_SetTextureAlphaMod(targetTexture, 128);  // Transparence
            SDL_RenderCopy(Renderer, targetTexture, NULL, &targetRect);
        }

        // Dessiner les cellules sélectionnées
        DrawSelectedCells(&play_grid, selected_cells, num_shots, targetTexture);

        SDL_RenderPresent(Renderer);  // Mettre à jour l'affichage
    }

    
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyTexture(backgroundTexture);
    //fin de jeux
    if(player_one.health==0){
        printf("player 2 win");
    }
    else{
        printf("player 1 win");
    }
}
