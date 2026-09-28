#include <emscripten/bind.h>
#include "../OverhaulFissionNet.h"

static void clearFuels(OverhaulFission::Settings &settings) {
  settings.fuels.clear();
}

static void addFuel(OverhaulFission::Settings &settings, double efficiency, int limit, int criticality, int heat, bool selfPriming) {
  auto &fuel(settings.fuels.emplace_back());
  fuel.efficiency = efficiency;
  fuel.limit = limit;
  fuel.criticality = criticality;
  fuel.heat = heat;
  fuel.selfPriming = selfPriming;
}

static void overhaulSetLimit(OverhaulFission::Settings &x, int index, int limit) {
  x.limits[index] = limit;
}

static void setSourceLimit(OverhaulFission::Settings &x, int index, int limit) {
  x.sourceLimits[index] = limit;
}

static emscripten::val overhaulGetData(const OverhaulFission::Sample &x) {
  return emscripten::val(emscripten::typed_memory_view(x.state.size(), x.state.data()));
}

static int overhaulGetShape(const OverhaulFission::Sample &x, int i) {
  return x.state.shape(i);
}

static int overhaulGetStride(const OverhaulFission::Sample &x, int i) {
  return x.state.strides()[i];
}

static double getOutput(const OverhaulFission::Sample &x) {
  return x.value.output;
}

static int getFuelUse(const OverhaulFission::Sample &x) {
  return x.value.nActiveCells;
}

static double overhaulGetEfficiency(const OverhaulFission::Sample &x) {
  return x.value.efficiency;
}

static int getIrradiatorFlux(const OverhaulFission::Sample &x) {
  return x.value.irradiatorFlux;
}

static emscripten::val overhaulGetLossHistory(const OverhaulFission::Opt &opt) {
  auto &data(opt.getLossHistory());
  return emscripten::val(emscripten::typed_memory_view(data.size(), data.data()));
}

EMSCRIPTEN_BINDINGS(FissionOpt) {
  emscripten::class_<OverhaulFission::Settings>("OverhaulFissionSettings")
    .constructor<>()
    .property("sizeX", &OverhaulFission::Settings::sizeX)
    .property("sizeY", &OverhaulFission::Settings::sizeY)
    .property("sizeZ", &OverhaulFission::Settings::sizeZ)
    .function("clearFuels", &clearFuels)
    .function("addFuel", &addFuel)
    .function("setLimit", &overhaulSetLimit)
    .function("setSourceLimit", &setSourceLimit)
    .property("goal", &OverhaulFission::Settings::goal)
    .property("controllable", &OverhaulFission::Settings::controllable)
    .property("symX", &OverhaulFission::Settings::symX)
    .property("symY", &OverhaulFission::Settings::symY)
    .property("symZ", &OverhaulFission::Settings::symZ);
  emscripten::class_<OverhaulFission::Sample>("OverhaulFissionSample")
    .function("getData", &overhaulGetData)
    .function("getShape", &overhaulGetShape)
    .function("getStride", &overhaulGetStride)
    .function("getOutput", &getOutput)
    .function("getFuelUse", &getFuelUse)
    .function("getEfficiency", &overhaulGetEfficiency)
    .function("getIrradiatorFlux", &getIrradiatorFlux);
  emscripten::class_<OverhaulFission::Opt>("OverhaulFissionOpt")
    .constructor<OverhaulFission::Settings&>()
    .function("stepInteractive", &OverhaulFission::Opt::stepInteractive)
    .function("needsRedrawBest", &OverhaulFission::Opt::needsRedrawBest)
    .function("needsReplotLoss", &OverhaulFission::Opt::needsReplotLoss)
    .function("getLossHistory", &overhaulGetLossHistory)
    .function("getBest", &OverhaulFission::Opt::getBest)
    .function("getNEpisode", &OverhaulFission::Opt::getNEpisode)
    .function("getNStage", &OverhaulFission::Opt::getNStage)
    .function("getNIteration", &OverhaulFission::Opt::getNIteration);
}
