# Info

  資料結構期末報告

  ## Contributors
  
  張清清 (W1114171002)
  陳偉殿 (W1114171003)

# Description

  The "cage" in the program is just a variable name. We can actually place them according to the behavior of the pet. For example, we can put some dogs in a large and comfortable place so that they can play and move without any restrictions. We can also provide shelter for small pets such as hamsters, chinchillas and rabbits. We can take care of them in appropriately sized shelter and allow them to play and live safely in it.

  The program is designed to help pet shop owners manage their pets. The program will autosave on every changes made to the data (always autosave after pet added or removed from a shelter) and when exiting the program. The program will save the pets data as a text file named "pets_data.txt" in the same directory as the program. The program will load the data from the "pets_data.txt" file when it is started.

# How to use

  ## For Windows
  1. Clone this repository or download the pet_shop.exe file into your local computer.
  2. Run the pet_shop.exe file.
      - If you see a warning message, click "More info" and then click "Run anyway".
      - Or you can run the program in your termimal by typing `.\pet_shop.exe`.

  ## Modify
  You can always customize the program according to your needs by modifying the source code in pet_shop.c, then you can use gcc compiler to recompile the program using the following command:
  ```
  gcc pet_shop.c -o pet_shop
  ```
  Then you can run the modified program by typing `.\pet_shop.exe`.
