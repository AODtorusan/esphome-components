
# ESPHome-components

This is a set of custom ESPHome components are/have-been used in my homeautomation system to expand on the default ESPHome functionality.

## Usage

Include the relevant external component in your device yaml config, as use it as outlined in the component's readme.

```yaml
  external_components:
    - source:
        type: git
        ref: esphome-latest
        url: https://github.com/AODtorusan/esphome-components
      components: [ "component_name" ]
```

There are a few relevant refs;

- master (branch-head): Latest development commit
- esphome-latest (tag): Latest commit that passed all component test compiles
- esphome-${version} (tag): Latest commit that passed all component test compiles for that specific esphome full version, eg esphome-2026.2.1
- esphome-${version_minor} (tag): Latest commit that passed all component test compiles for that specific esphome minor version, eg esphome-2026.2

The components are periodically tested with the latest available version of esphome.

## Custom components

### KNX

This component adds KNX TP functionality to esphome through using a TPUART2 interface to connect to the KNX bus and listen to and publish KNX group messages.

See: [KNX component](components/knx/README.md)
