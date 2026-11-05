#include <Arduino.h>
#include <unity.h>
#include <time.h>
#include <Relay.h>
#include <IrrigationController.h>

#ifndef NATIVE_TEST
#include <sys/time.h>
#endif

Relay r1(14, "Zona 1");
Relay r2(13, "Zona 2");
Relay r3(12, "Extra");

static void setClock(int hh, int mm, int ss) {
    struct tm t = {};
    t.tm_year = 2026 - 1900;
    t.tm_mon  = 10;      // noviembre (0-11)
    t.tm_mday = 2;       // lunes
    t.tm_hour = hh; t.tm_min = mm; t.tm_sec = ss;
#ifdef NATIVE_TEST
    t.tm_wday = 1;       // lunes
    fakeClock = t;
    fakeClockValid = true;
#else
    t.tm_isdst = 0;
    struct timeval tv = { .tv_sec = mktime(&t), .tv_usec = 0 };
    settimeofday(&tv, nullptr);
#endif
}

static void invalidateClock() {
#ifdef NATIVE_TEST
    fakeClockValid = false;
#else
    struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };
    settimeofday(&tv, nullptr);
#endif
}

static long secs(int h, int m) { return h * 3600L + m * 60L; }

static void addRelays(IrrigationController &c) {
    c.addIrrigationRelay(&r1);
    c.addIrrigationRelay(&r2);
}

static void assertWatering(IrrigationController &c, bool expected) {
    TEST_ASSERT_EQUAL(expected, c.getIsWatering());
    TEST_ASSERT_EQUAL(expected, r1.isActive());
    TEST_ASSERT_EQUAL(expected, r2.isActive());
}

void setUp() { r1.off(); r2.off(); r3.off(); }
void tearDown() {}

void test_PU05_maximo_dos_reles() {
    IrrigationController c;
    TEST_ASSERT_TRUE(c.addIrrigationRelay(&r1));
    TEST_ASSERT_TRUE(c.addIrrigationRelay(&r2));
    TEST_ASSERT_FALSE(c.addIrrigationRelay(&r3));
}

void test_PU06_dentro_de_ventana_riega() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);               // lunes
    setClock(8, 10, 0);
    c.update();
    assertWatering(c, true);
}

void test_PU07_limite_inferior_inclusivo() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);
    setClock(8, 0, 0);
    c.update();
    assertWatering(c, true);
}

void test_PU08_limite_superior_exclusivo() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);
    setClock(8, 30, 0);
    c.update();
    assertWatering(c, false);
}

void test_PU09_ventana_cruza_medianoche() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(23, 0), secs(1, 0));
    c.setDayActive(1, true);
    setClock(0, 30, 0);
    c.update();
    assertWatering(c, true);
    setClock(12, 0, 0);
    c.update();
    assertWatering(c, false);
}

void test_PU10_dia_inhabilitado_no_riega() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(2, true);               // solo martes
    setClock(8, 10, 0);                    // es lunes
    c.update();
    assertWatering(c, false);
}

void test_PU11_inicio_igual_a_fin_no_riega() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 0));
    c.setDayActive(1, true);
    setClock(8, 0, 0);
    c.update();
    assertWatering(c, false);
}

void test_PU12_dias_fuera_de_rango_ignorados() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(0, true);
    c.setDayActive(8, true);
    setClock(8, 10, 0);
    c.update();
    assertWatering(c, false);
}

void test_PU13_manual_tiene_precedencia() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);
    setClock(12, 0, 0);                    // fuera de la ventana
    c.enableManualMode();
    c.update();
    TEST_ASSERT_TRUE(c.getManualMode());
    assertWatering(c, true);
}

void test_PU14_desactivar_manual_apaga_reles() {
    IrrigationController c; addRelays(c);
    c.enableManualMode();
    c.disableManualMode();
    TEST_ASSERT_FALSE(c.getManualMode());
    assertWatering(c, false);
}

void test_PU15_amanecer_atardecer() {
    IrrigationController c; addRelays(c);
    c.setStartAtSunrise(true);             // por defecto 07:00
    c.setStopAtSunset(true);               // por defecto 19:30
    c.setDayActive(1, true);
    setClock(12, 0, 0);
    c.update();
    assertWatering(c, true);
    setClock(6, 0, 0);                     // antes del amanecer
    c.update();
    assertWatering(c, false);
}

void test_PU16_sin_hora_valida_no_riega() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);
    invalidateClock();
    c.update();
    assertWatering(c, false);
}

void test_PU17_clear_schedule_detiene_riego() {
    IrrigationController c; addRelays(c);
    c.setSchedule(secs(8, 0), secs(8, 30));
    c.setDayActive(1, true);
    setClock(8, 10, 0);
    c.update();
    assertWatering(c, true);
    c.clearSchedule();
    c.update();
    assertWatering(c, false);
}

static int runTests() {
    UNITY_BEGIN();
    RUN_TEST(test_PU05_maximo_dos_reles);
    RUN_TEST(test_PU06_dentro_de_ventana_riega);
    RUN_TEST(test_PU07_limite_inferior_inclusivo);
    RUN_TEST(test_PU08_limite_superior_exclusivo);
    RUN_TEST(test_PU09_ventana_cruza_medianoche);
    RUN_TEST(test_PU10_dia_inhabilitado_no_riega);
    RUN_TEST(test_PU11_inicio_igual_a_fin_no_riega);
    RUN_TEST(test_PU12_dias_fuera_de_rango_ignorados);
    RUN_TEST(test_PU13_manual_tiene_precedencia);
    RUN_TEST(test_PU14_desactivar_manual_apaga_reles);
    RUN_TEST(test_PU15_amanecer_atardecer);
    RUN_TEST(test_PU16_sin_hora_valida_no_riega);
    RUN_TEST(test_PU17_clear_schedule_detiene_riego);
    return UNITY_END();
}

#ifdef NATIVE_TEST
int main() { runTests(); return 0; }
#else
void setup() {
    delay(2000);
    setenv("TZ", "UTC0", 1);
    tzset();
    runTests();
}
void loop() {}
#endif