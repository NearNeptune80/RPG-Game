// inventory.cpp
#include "inventory.h"
#include <algorithm>
#include <iostream>

const int MAX_INVENTORY_SIZE = 28;

inventory::inventory(SDL_Renderer* renderer)
{
    equippedItems.assign(8, item(0, 0, 0, "", -1, "", "", 0, renderer));
    storedItems.assign(MAX_INVENTORY_SIZE, item());
}

inventory::~inventory() {}

void inventory::renderInventory(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, TTF_Font* invInfoFont, const std::string& playerName, int playerLvl, int atkLvl, int defLvl, int hpLvl)
{
    // Render equipment and inventory containers
    SDL_Rect equipmentContainer = { 0, 112, 390, 672 };
    SDL_Rect inventoryContainer = { 1210, 112, 390, 672 };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xFF);
    SDL_RenderFillRect(renderer, &equipmentContainer);
    SDL_RenderFillRect(renderer, &inventoryContainer);

    // Render character info box
    SDL_Rect charaInfoBox = { 14, 126, 362, 80 };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
    SDL_RenderFillRect(renderer, &charaInfoBox);

    // Render player name and level
    auto renderText = [&](const std::string& text, int x, int y) {
        SDL_Color textColor = { 255, 255, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Blended(invInfoFont, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        };

    renderText(playerName, charaInfoBox.x + 10, charaInfoBox.y + 10);
    renderText("Level " + std::to_string(playerLvl), charaInfoBox.x + 10, charaInfoBox.y + charaInfoBox.h - 37);

    // Equipment Boxes
    int boxSize = 80;
    int padding = 14;
    std::vector<int> boxesPerRow = { 1, 3, 3, 1 };
    int startY = 220;
    int currentIndex = 0;

    SDL_Rect equipmentBoxes[8];
    for (size_t row = 0; row < boxesPerRow.size(); ++row)
    {
        int boxesInThisRow = boxesPerRow[row];
        int totalWidth = boxesInThisRow * boxSize + (boxesInThisRow - 1) * padding;
        int startX = (equipmentContainer.w - totalWidth) / 2;

        for (int i = 0; i < boxesInThisRow; ++i)
        {
            if (currentIndex >= 8) break;
            int x = startX + i * (boxSize + padding);
            int y = startY + row * (boxSize + padding);
            equipmentBoxes[currentIndex] = { x, y, boxSize, boxSize };

            // Highlight if hovered
            bool isHovered = (mouseX >= x && mouseX <= x + boxSize && mouseY >= y && mouseY <= y + boxSize);
            SDL_SetRenderDrawColor(renderer, isHovered ? 150 : 100, 100, 100, 0xFF);
            SDL_RenderFillRect(renderer, &equipmentBoxes[currentIndex]);

            // Render item image based on equipmentOrder
            int itemIndex = equipmentOrder[currentIndex];

            // **Skip rendering if this slot is being dragged from**
            bool isSourceSlot = isDragging &&
                sourceSlotType == "equipment" &&
                sourceIndex == currentIndex;

            if (!isSourceSlot && !equippedItems[itemIndex].imagelocation.empty() && equippedItems[itemIndex].texture)
            {
                SDL_Rect itemRect = { x + 5, y + 5, boxSize - 10, boxSize - 10 };
                SDL_RenderCopy(renderer, equippedItems[itemIndex].texture.get(), NULL, &itemRect);
            }
            currentIndex++;
        }
    }

    // Inventory Grid
    int index = 0;
    for (int j = 126; j <= 690; j += 94)
    {
        for (int i = 1224; i < 1600; i += 94)
        {
            SDL_Rect inventoryGrid = { i, j, 80, 80 };
            bool isHovered = (mouseX >= i && mouseX < i + 80 && mouseY >= j && mouseY < j + 80);
            SDL_SetRenderDrawColor(renderer, isHovered ? 150 : 100, 100, 100, 0xFF);
            SDL_RenderFillRect(renderer, &inventoryGrid);

            // **Skip rendering if this slot is being dragged from**
            bool isSourceSlot = isDragging &&
                sourceSlotType == "inventory" &&
                sourceIndex == index;

            if (!isSourceSlot && index < storedItems.size() && storedItems[index].texture)
            {
                SDL_Rect itemRect = { i + 5, j + 5, 70, 70 };
                SDL_RenderCopy(renderer, storedItems[index].texture.get(), NULL, &itemRect);
            }
            index++;
        }
    }

    // **Updated reference to use the last equipment box (index 7)**
    if (currentIndex >= 8) // Ensure equipmentBoxes[7] is populated
    {
        renderLvlUpBoxes(renderer, font, mouseX, mouseY, equipmentBoxes[7].y, equipmentBoxes[7].h, atkLvl, defLvl, hpLvl);
    }

    // Render item descriptions
    for (int i = 0; i < 8; ++i)
    {
        int itemIndex = equipmentOrder[i];
        if (mouseX >= equipmentBoxes[i].x && mouseX <= equipmentBoxes[i].x + boxSize &&
            mouseY >= equipmentBoxes[i].y && mouseY <= equipmentBoxes[i].y + boxSize)
        {
            renderDescription(renderer, mouseX, mouseY, font, equipmentBoxes[i].x, equipmentBoxes[i].y);
        }
    }

    index = 0;
    for (int j = 126; j <= 690; j += 94)
    {
        for (int i = 1224; i < 1600; i += 94)
        {
            if (mouseX >= i && mouseX <= i + 80 && mouseY >= j && mouseY <= j + 80)
            {
                renderDescription(renderer, mouseX, mouseY, font, i, j);
            }
            index++;
        }
    }

    // **Render the dragged item following the cursor**
    if (isDragging && draggedItem.texture)
    {
        SDL_Rect itemRect = { mouseX - 40, mouseY - 40, 80, 80 };
        SDL_RenderCopy(renderer, draggedItem.texture.get(), NULL, &itemRect);
    }
}

void inventory::renderDescription(SDL_Renderer* renderer, int mouseX, int mouseY, TTF_Font* font, int i, int j)
{
    item descItem = getItem(mouseX, mouseY);

    if (!descItem.name.empty())
    {
        // Retrieve screen dimensions
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

        // Define description box dimensions
        const int descWidth = 200;
        const int descHeight = 300;
        const int padding = 10;

        // Calculate initial x position
        int descX = (i > 800) ? i - 214 : i + 94;

        // Calculate initial y position
        int descY = j;

        // Adjust y position if the description box would go beyond the screen height
        if (descY + descHeight > screenHeight)
        {
            descY = j - descHeight + 80;
            // Ensure the description box doesn't go above the screen
            if (descY < 0)
            {
                descY = padding;
            }
        }

        SDL_Rect descBox = { descX, descY, descWidth, descHeight };
        SDL_Rect descInterior = { descBox.x + 10, descBox.y + 10, descBox.w - 20, descBox.h - 20 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
        SDL_RenderFillRect(renderer, &descBox);
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xFF);
        SDL_RenderFillRect(renderer, &descInterior);

        SDL_Color textColor = { 255, 255, 255 };
        int offsetY = descInterior.y + 5;

        auto renderTextLine = [&](const std::string& text) {
            SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), textColor, descInterior.w - 10);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { descInterior.x + 5, offsetY, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            offsetY += textRect.h + 5;
            };

        renderTextLine(descItem.name);
        if (descItem.atk != 0) renderTextLine("Attack: " + std::to_string(descItem.atk));
        if (descItem.def != 0) renderTextLine("Defense: " + std::to_string(descItem.def));
        if (descItem.price != 0) renderTextLine("Price: " + std::to_string(descItem.price));
        if (descItem.lvl != 0) renderTextLine("Level: " + std::to_string(descItem.lvl));
        renderTextLine(descItem.description);
    }
}

void inventory::renderLvlUpBoxes(SDL_Renderer* renderer, TTF_Font* font, int mouseX, int mouseY, int previousBoxY, int previousBoxH, int atkLvl, int defLvl, int hpLvl)
{
    int nPreviousBoxY = previousBoxY + previousBoxH;
    int boxWidth = 362;

    for (int i = 0; i < 3; i++)
    {
        // Further increased y-offsets to lower the level-up boxes
        SDL_Rect lvlUpBox = { 14, nPreviousBoxY + 35, boxWidth, 40 };       // Changed from +80 to +100
        SDL_Rect lvlUpIcon = { 19, nPreviousBoxY + 40, 30, 30 };           // Changed from +85 to +105

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0xFF);
        SDL_RenderFillRect(renderer, &lvlUpBox);
        SDL_SetRenderDrawColor(renderer, (mouseX >= lvlUpIcon.x && mouseX <= lvlUpIcon.x + lvlUpIcon.w &&
            mouseY >= lvlUpIcon.y && mouseY <= lvlUpIcon.y + lvlUpIcon.h) ? 200 : 150, 150, 150, 200);
        SDL_RenderFillRect(renderer, &lvlUpIcon);

        nPreviousBoxY += 54;
    }

    std::vector<std::string> statNames = { "Attack", "Defense", "Health" };
    std::vector<int> statValues = { atkLvl, defLvl, hpLvl };

    previousBoxY += previousBoxH;

    for (int i = 0; i < 3; i++)
    {
        SDL_Color textColor = { 255, 255, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, statNames[i].c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Surface* statValueSurface = TTF_RenderText_Blended(font, std::to_string(statValues[i]).c_str(), textColor);
        SDL_Texture* statValueTexture = SDL_CreateTextureFromSurface(renderer, statValueSurface);

        SDL_Rect textRect = { 54, previousBoxY + 37, textSurface->w + 10, textSurface->h + 5 };
        SDL_Rect statValueRect = { boxWidth - 20, previousBoxY + 37, statValueSurface->w + 5, statValueSurface->h + 5 };

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderCopy(renderer, statValueTexture, NULL, &statValueRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(statValueSurface);
        SDL_DestroyTexture(statValueTexture);

        previousBoxY += 54;
    }
}



int inventory::thingHovered(int mouseX, int mouseY)
{
    int previousBoxY = 502;
    int previousBoxH = 80;
    int nPreviousBoxY = previousBoxY + previousBoxH;

    for (int i = 0; i < 3; i++)
    {
        SDL_Rect lvlUpIcon = { 19, nPreviousBoxY + 40, 30, 30 };
        if (mouseX >= lvlUpIcon.x && mouseX <= lvlUpIcon.x + lvlUpIcon.w &&
            mouseY >= lvlUpIcon.y && mouseY <= lvlUpIcon.y + lvlUpIcon.h)
        {
            return i;
        }
        nPreviousBoxY += 54;
    }
    return -1;
}

std::optional<item> inventory::dragAndDrop(int mouseX, int mouseY, SDL_Renderer* renderer, bool buttonHeld, int playerLvl)
{
    if (buttonHeld && !isDragging)
    {
        auto [slotType, index] = getSlotTypeAndIndex(mouseX, mouseY);
        if (!slotType.empty() && index >= 0)
        {
            sourceSlotType = slotType;
            sourceIndex = index;

            if (slotType == "equipment")
            {
                if (index >= equipmentOrder.size()) return std::nullopt; // Safety check
                int actualIndex = equipmentOrder[index];
                if (actualIndex >= equippedItems.size()) return std::nullopt; // Safety check
                draggedItem = equippedItems[actualIndex];
            }
            else if (slotType == "inventory")
            {
                if (index >= storedItems.size()) return std::nullopt; // Safety check
                draggedItem = storedItems[index];
            }

            isDragging = true;
        }
    }
    else if (!buttonHeld && isDragging)
    {
        isDragging = false;
        auto [targetSlotType, targetIndex] = getSlotTypeAndIndex(mouseX, mouseY);
        bool dropSuccess = false;

        if (sourceSlotType == "inventory")
        {
            if (targetSlotType == "inventory" && targetIndex < storedItems.size())
            {
                // Swap between inventory slots
                std::swap(storedItems[sourceIndex], storedItems[targetIndex]);
                dropSuccess = true;
            }
            else if (targetSlotType == "equipment" && targetIndex < 8)
            {
                // Check if the dragged item can be equipped in the target equipment slot
                if (isValidEquipmentSlot(draggedItem, equipmentOrder[targetIndex]))
                {
                    // **Enforce level restriction**
                    if (draggedItem.lvl <= playerLvl)
                    {
                        // Swap only if the target equipment slot is empty or has an item of the same type
                        if (equippedItems[equipmentOrder[targetIndex]].name.empty() ||
                            draggedItem.itemType == equippedItems[equipmentOrder[targetIndex]].itemType)
                        {
                            std::swap(storedItems[sourceIndex], equippedItems[equipmentOrder[targetIndex]]);
                            equipmentChanged = true;
                            dropSuccess = true;
                        }
                        else
                        {
                            // Provide feedback that the item types do not match
                            std::cout << "Item types do not match. Cannot equip " << draggedItem.name << "." << std::endl;
                        }
                    }
                    else
                    {
                        // Provide feedback that the item's level is too high
                        std::cout << "Cannot equip " << draggedItem.name << ". Required level: " << draggedItem.lvl
                            << ", Player level: " << playerLvl << std::endl;
                    }
                }
            }
            // If not dropped successfully, item remains in original slot
        }
        else if (sourceSlotType == "equipment")
        {
            if (targetSlotType == "inventory" && targetIndex < storedItems.size())
            {
                if (storedItems[targetIndex].lvl <= playerLvl)
                {
                    // Swap between equipment slot and inventory slot
                    if (storedItems[targetIndex].name.empty() ||
                        draggedItem.itemType == storedItems[targetIndex].itemType)
                    {
                        std::swap(equippedItems[equipmentOrder[sourceIndex]], storedItems[targetIndex]);
                        equipmentChanged = true;
                        dropSuccess = true;
                    }
				}
                else
                {
                    // Provide feedback that the item's level is too high
                    std::cout << "Cannot equip " << storedItems[targetIndex].name << ". Required level: " << storedItems[targetIndex].lvl
                        << ", Player level: " << playerLvl << std::endl;
                }
            }
            else if (targetSlotType == "equipment" && targetIndex < 8)
            {
                // **Enforce level restriction for swapping equipment slots**
                if (draggedItem.lvl <= playerLvl)
                {
                    // Swapping between two equipment slots
                    int targetEquipmentIndex = equipmentOrder[targetIndex];
                    item& targetItem = equippedItems[targetEquipmentIndex];

                    // Check if each item can be equipped in the other's slot
                    if (isValidEquipmentSlot(draggedItem, targetEquipmentIndex) &&
                        isValidEquipmentSlot(targetItem, equipmentOrder[sourceIndex]))
                    {
                        // **Additional level check for the target item**
                        if (targetItem.lvl <= playerLvl)
                        {
                            std::swap(equippedItems[equipmentOrder[sourceIndex]], equippedItems[targetEquipmentIndex]);
                            equipmentChanged = true;
                            dropSuccess = true;
                        }
                        else
                        {
                            std::cout << "Cannot equip " << targetItem.name << ". Required level: " << targetItem.lvl
                                << ", Player level: " << playerLvl << std::endl;
                        }
                    }
                }
                else
                {
                    // Provide feedback that the dragged item's level is too high
                    std::cout << "Cannot equip " << draggedItem.name << ". Required level: " << draggedItem.lvl
                        << ", Player level: " << playerLvl << std::endl;
                }
            }
            // If not dropped successfully, item remains in original slot
        }

        // Reset the dragged item and source information
        draggedItem = item();
        sourceSlotType.clear();
        sourceIndex = -1;
    }

    // Render the dragged item if dragging
    if (isDragging && draggedItem.texture)
    {
        SDL_Rect itemRect = { mouseX - 40, mouseY - 40, 80, 80 };
        SDL_RenderCopy(renderer, draggedItem.texture.get(), NULL, &itemRect);
    }

    return std::nullopt;
}

bool inventory::addItem(const item& newItem)
{
    for (auto& itm : storedItems)
    {
        if (itm.name.empty())
        {
            itm = newItem;
            return true;
        }
    }
    std::cout << "Inventory is full!" << std::endl;
    return false;
}

void inventory::removeItem(int index)
{
    if (index >= 0 && index < storedItems.size())
    {
        storedItems.erase(storedItems.begin() + index);
    }
}

item inventory::getItem(int mouseX, int mouseY)
{
    auto [slotType, index] = getSlotTypeAndIndex(mouseX, mouseY);
    if (slotType == "equipment" && index >= 0 && index < 8)
    {
        int actualIndex = equipmentOrder[index];
        return equippedItems[actualIndex];
    }
    else if (slotType == "inventory" && index >= 0 && index < storedItems.size())
    {
        return storedItems[index];
    }
    return item();
}

std::pair<std::string, int> inventory::getSlotTypeAndIndex(int mouseX, int mouseY)
{
    int boxSize = 80;
    int padding = 14;
    std::vector<int> boxesPerRow = { 1, 3, 3, 1 };
    // Removed local equipmentOrder definition
    int startY = 220;
    int currentIndex = 0;

    for (size_t row = 0; row < boxesPerRow.size(); ++row)
    {
        int boxesInThisRow = boxesPerRow[row];
        int totalWidth = boxesInThisRow * boxSize + (boxesInThisRow - 1) * padding;
        int startX = (390 - totalWidth) / 2;

        for (int i = 0; i < boxesInThisRow; ++i)
        {
            if (currentIndex >= 8) break;
            int x = startX + i * (boxSize + padding);
            int y = startY + row * (boxSize + padding);
            if (mouseX >= x && mouseX < x + boxSize && mouseY >= y && mouseY < y + boxSize)
            {
                return { "equipment", currentIndex };
            }
            currentIndex++;
        }
    }

    // Inventory Slots
    if (mouseX >= 1210 && mouseX < 1210 + 390 &&
        mouseY >= 112 && mouseY <= 112 + 672)
    {
        int relativeX = mouseX - 1210;
        int relativeY = mouseY - 112;
        int col = relativeX / 94;
        int row = relativeY / 94;
        int index = row * 4 + col;
        if (col < 4 && row < 7 && index < storedItems.size())
        {
            return { "inventory", index };
        }
    }

    return { "", -1 };
}

bool inventory::isValidEquipmentSlot(const item& itm, int slotIndex)
{
    return (itm.itemType == slotIndex) || (itm.itemType == 6 && (slotIndex == 6 || slotIndex == 7));
}

bool inventory::isInventoryFull()
{
    return std::none_of(storedItems.begin(), storedItems.end(), [](const item& itm) {
        return itm.name.empty();
        });
}