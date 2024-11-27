<script setup lang="ts">
import { connectServer } from '@/ble/device';
import { writeServo, writeMotorA } from '@/ble/rctrl';
import { ref, watchEffect, watch } from 'vue';
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

const servoAngle = ref(0);
const motorASpeed = ref(0);

watch(servoAngle, (angle) => {
  doOperation(() => writeServo(angle));
});

watch(motorASpeed, (speed) => {
  doOperation(() => writeMotorA(speed));
});

</script>

<template>
  <main>
    <h1>Home</h1>
    <button @click="connect">Connect</button>
    <p>Status: <span>{{ connected ? 'Connected' : 'Not Connected' }}</span></p>
    <br>
    <p>
      Servo Angle:
      <slider v-model:value="servoAngle" :label="true" :min="-90" :max="90" />
    </p>
    <p>
      Motor A Speed:
      <slider v-model:value="motorASpeed" :label="true" :min="-100" :max="100" />
    </p>
  </main>
</template>
