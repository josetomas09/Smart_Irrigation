#include <Arduino.h>
#include <unity.h>
#include <Relay.h>

#ifndef NATIVE_TEST
#include "soc/gpio_reg.h"
#endif

static const uint8_t TEST_PIN = 14;

static uint8_t outputLevel(uint8_t pin) {
#ifdef NATIVE_TEST
    return fakePinLevel[pin];                       // GPIO simulado
#else
    return (REG_READ(GPIO_OUT_REG) >> pin) & 0x1;   // registro de salida del ESP32
#endif
}

void setUp() {}
void tearDown() {}

void test_PU01_constructor_deja_rele_desenergizado() {
    Relay r(TEST_PIN, "Test");
    TEST_ASSERT_FALSE(r.isActive());
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(HIGH, outputLevel(TEST_PIN),
        "El rele queda energizado al construirse (logica activa en bajo)");
}

void test_PU02_on_activa_rele() {
    Relay r(TEST_PIN, "Test");
    r.off();
    r.on();
    TEST_ASSERT_TRUE(r.isActive());
    TEST_ASSERT_EQUAL_UINT8(LOW, outputLevel(TEST_PIN));
}

void test_PU03_off_desactiva_rele() {
    Relay r(TEST_PIN, "Test");
    r.on();
    r.off();
    TEST_ASSERT_FALSE(r.isActive());
    TEST_ASSERT_EQUAL_UINT8(HIGH, outputLevel(TEST_PIN));
}

void test_PU04_toggle_alterna_estado() {
    Relay r(TEST_PIN, "Test");
    r.off();
    r.toggle();
    TEST_ASSERT_TRUE(r.isActive());
    r.toggle();
    TEST_ASSERT_FALSE(r.isActive());
}

static int runTests() {
    UNITY_BEGIN();
    RUN_TEST(test_PU01_constructor_deja_rele_desenergizado);
    RUN_TEST(test_PU02_on_activa_rele);
    RUN_TEST(test_PU03_off_desactiva_rele);
    RUN_TEST(test_PU04_toggle_alterna_estado);
    return UNITY_END();
}

#ifdef NATIVE_TEST
int main() { runTests(); return 0; }
#else
void setup() { delay(2000); runTests(); }
void loop() {}
#endif