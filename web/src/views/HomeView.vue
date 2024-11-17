<script setup lang="ts">
import { connectServer } from '@/ble/device';
import { writeServo } from '@/ble/rctrl';
import { ref, watchEffect } from 'vue';
import { Slider } from 'tdesign-mobile-vue';

const connected = ref(false);

const connect = async () => {
  await connectServer();
  connected.value = true;
};

const operating = ref(false);
async function doOperation(op: () => Promise<any>) {
  if (!operating.value) {
    operating.value = true;
    await op();
    operating.value = false;
  }
}

const sliderValue = ref(0);

watchEffect(async () => {
  if (connected.value) {
    if (!operating.value) {
      doOperation(async () => await writeServo(sliderValue.value));
    }
  }
});
</script>

<template>
  <main>
    <h1>Home</h1>
    <button @click="connect">Connect</button>
    <p>Status: <span>{{ connected ? 'Connected' : 'Not Connected' }}</span></p>
    <br>
    <slider v-model:value="sliderValue" :label="true" :min="-90" :max="90" />
  </main>
</template>
