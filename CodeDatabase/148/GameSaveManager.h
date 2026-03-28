#ifndef GAMESAVEMANAGER_H
#define GAMESAVEMANAGER_H

#include <string>

class Game;

/**
 * @file GameSaveManager.h
 * @brief Менеджер сохранения и загрузки игры
 * 
 * Отвечает за сериализацию состояния игры в файл и восстановление из файла.
 * Использует текстовый формат для простоты отладки и переносимости.
 */

/**
 * @class GameSaveManager
 * @brief Статический класс для управления сохранениями
 * 
 * Функционал:
 * - Сохранение полного состояния игры
 * - Загрузка состояния из файла
 * - Валидация сохранённых данных
 * - Обработка ошибок файловой системы
 */
class GameSaveManager {
private:
    static const std::string SAVE_DIRECTORY;  ///< Директория для сохранений
    static const std::string SAVE_EXTENSION;  ///< Расширение файлов сохранений
    
    /**
     * @brief Создать директорию для сохранений если её нет
     */
    static void ensureSaveDirectoryExists();
    
    /**
     * @brief Получить полный путь к файлу сохранения
     * @param saveName Имя сохранения
     * @return Полный путь к файлу
     */
    static std::string getSavePath(const std::string& saveName);
    
    /**
     * @brief Проверить существование сохранения
     * @param saveName Имя сохранения
     * @return true если файл существует
     */
    static bool saveExists(const std::string& saveName);
    
    /**
     * @brief Валидация данных сохранения
     * @param data Строка с данными
     * @return true если данные корректны
     */
    static bool validateSaveData(const std::string& data);
    
public:
    /**
     * @brief Сохранить игру
     * @param game Указатель на игру
     * @param saveName Имя сохранения
     * @throws InvalidGameStateException если состояние игры некорректно
     * @throws FileWriteException если не удалось записать файл
     */
    static void saveGame(Game* game, const std::string& saveName);
    
    /**
     * @brief Загрузить игру
     * @param game Указатель на игру
     * @param saveName Имя сохранения
     * @throws FileNotFoundException если файл не найден
     * @throws CorruptedSaveException если файл повреждён
     * @throws FileReadException если не удалось прочитать файл
     */
    static void loadGame(Game* game, const std::string& saveName);
    
    /**
     * @brief Безопасное сохранение игры с обработкой ошибок
     * @param game Указатель на игру
     * @param saveName Имя сохранения
     * @param errorMessage Выходной параметр с описанием ошибки
     * @return true если сохранение успешно, false при ошибке
     * 
     * Этот метод ловит все исключения и возвращает false вместо их выброса.
     * Игра продолжает работать даже при ошибке сохранения.
     */
    static bool trySaveGame(Game* game, const std::string& saveName, std::string& errorMessage);
    
    /**
     * @brief Безопасная загрузка игры с обработкой ошибок
     * @param game Указатель на игру
     * @param saveName Имя сохранения
     * @param errorMessage Выходной параметр с описанием ошибки
     * @return true если загрузка успешна, false при ошибке
     * 
     * Этот метод ловит все исключения и возвращает false вместо их выброса.
     * При ошибке загрузки игра остаётся в текущем состоянии.
     */
    static bool tryLoadGame(Game* game, const std::string& saveName, std::string& errorMessage);
};

#endif // GAMESAVEMANAGER_H


