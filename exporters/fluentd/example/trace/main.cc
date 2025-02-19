// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"

// Using an exporter that simply dumps span data to stdout.
#include "foo_library/foo_library.h"
#include "opentelemetry/exporters/fluentd/trace/fluentd_exporter.h"

namespace sdktrace = opentelemetry::sdk::trace;
namespace nostd = opentelemetry::nostd;

#define HAVE_CONSOLE_LOG

namespace {
void initTracer() {
  opentelemetry::exporter::fluentd::common::FluentdExporterOptions options;
  options.endpoint = "tcp://localhost:24222";

  auto exporter = std::unique_ptr<sdktrace::SpanExporter>(
      new opentelemetry::exporter::fluentd::trace::FluentdExporter(options));
  auto processor = std::unique_ptr<sdktrace::SpanProcessor>(
      new sdktrace::SimpleSpanProcessor(std::move(exporter)));
  auto provider = nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
      new sdktrace::TracerProvider(std::move(processor)));

  // Set the global trace provider
  opentelemetry::trace::Provider::SetTracerProvider(provider);
}
} // namespace

int main() {
  // Removing this line will leave the default noop TracerProvider in place.
  initTracer();

  foo_library();
}
