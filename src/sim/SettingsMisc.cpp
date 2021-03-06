#include "SettingsMisc.h"

#include "Settings.h"
#include "SimUtilities.h"

namespace mms {

const QString SettingsMisc::GROUP = "misc";
const QString SettingsMisc::KEY_RECENT_MAZE_ALGO = "recent-maze-algo";
const QString SettingsMisc::KEY_RECENT_MOUSE_ALGO = "recent-mouse-algo";
const QString SettingsMisc::KEY_RECENT_WINDOW_WIDTH = "recent-window-width";
const QString SettingsMisc::KEY_RECENT_WINDOW_HEIGHT = "recent-window-height";
const QString SettingsMisc::KEY_FONT_IMAGE_PATH_COMBO_BOX_VALUE =
    "font-image-path-combo-box-value";
const QString SettingsMisc::KEY_FONT_IMAGE_PATH_LINE_EDIT_VALUE =
    "font-image-path-line-edit-value";
const QString SettingsMisc::KEY_FONT_IMAGE_PATH_COMBO_BOX_SELECTED =
    "font-image-path-combo-box-selected";

QString SettingsMisc::getRecentMazeAlgo() {
    return getValue(KEY_RECENT_MAZE_ALGO);
}

void SettingsMisc::setRecentMazeAlgo(const QString& name) {
    setValue(KEY_RECENT_MAZE_ALGO, name);
}

QString SettingsMisc::getRecentMouseAlgo() {
    return getValue(KEY_RECENT_MOUSE_ALGO);
}

void SettingsMisc::setRecentMouseAlgo(const QString& name) {
    setValue(KEY_RECENT_MOUSE_ALGO, name);
}

int SettingsMisc::getRecentWindowWidth() {
    return getNumber(KEY_RECENT_WINDOW_WIDTH, 1200);
}

void SettingsMisc::setRecentWindowWidth(int width) {
    setValue(KEY_RECENT_WINDOW_WIDTH, QString::number(width));
}

int SettingsMisc::getRecentWindowHeight() {
    return getNumber(KEY_RECENT_WINDOW_HEIGHT, 600);
}

void SettingsMisc::setRecentWindowHeight(int height) {
    setValue(KEY_RECENT_WINDOW_HEIGHT, QString::number(height));
}

QString SettingsMisc::getFontImagePathComboBoxValue() {
    return getValue(KEY_FONT_IMAGE_PATH_COMBO_BOX_VALUE);
}

void SettingsMisc::setFontImagePathComboBoxValue(QString value) {
    setValue(KEY_FONT_IMAGE_PATH_COMBO_BOX_VALUE, value);
}

QString SettingsMisc::getFontImagePathLineEditValue() {
    return getValue(KEY_FONT_IMAGE_PATH_LINE_EDIT_VALUE);
}

void SettingsMisc::setFontImagePathLineEditValue(QString value) {
    setValue(KEY_FONT_IMAGE_PATH_LINE_EDIT_VALUE, value);
}

bool SettingsMisc::getFontImagePathComboBoxSelected() {
    return getValue(KEY_FONT_IMAGE_PATH_COMBO_BOX_SELECTED) == "true";
}

void SettingsMisc::setFontImagePathComboBoxSelected(bool selected) {
    QString value = selected ? "true" : "false";
    setValue(KEY_FONT_IMAGE_PATH_COMBO_BOX_SELECTED, value);
}

int SettingsMisc::getNumber(QString key, int defaultValue) {
    int number = -1;
    QString value = getValue(key);
    if (SimUtilities::isInt(value)) {
        number = SimUtilities::strToInt(value);
    }
    if (number < 0) {
        number = defaultValue;
        setValue(key, QString::number(number));
    }
    return number;
}

QString SettingsMisc::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void SettingsMisc::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

} // namespace mms
