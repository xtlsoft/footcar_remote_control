<script setup lang="ts">
import { connectServer, onDisconnectServer, useServer, globalServer } from '@/ble/device';
import { writeServo, writeMotorA, writeMotorB } from '@/ble/rctrl';
import { computed, ref, watch } from 'vue';
import { Slider, Button as TButton, Switch as TSwitch, Cell } from 'tdesign-mobile-vue';
import { Joystick, JoystickComponent } from 'vue-joystick-component';
import { Debounce, DelayedMutex } from '@/ble/utils';

const connected = computed(() => globalServer.value !== null);

const connect = async () => {
  await connectServer();
};

const servoAngle = ref(0);
const motorASpeed = ref(0);
const motorBSpeed = ref(0);

const autoReturnZero = ref(true);

function speedCurve(orig_speed: number): number {
  const factor = 0.35;
  return Math.round(Math.sign(orig_speed) * Math.pow(Math.abs(orig_speed), factor) * Math.pow(100, 1 - factor));
}

const realSpeedA = ref(0);
const realSpeedB = ref(0);

const mutex = new DelayedMutex(20);

const angleDebounce = new Debounce();
watch(servoAngle, async (angle) => {
  angleDebounce.call(async () => {
    await mutex.acquire();
    await writeServo(angle);
    mutex.release();
  });
});

const motorADebounce = new Debounce();
watch(motorASpeed, async (speed) => {
  motorADebounce.call(async () => {
    await mutex.acquire();
    realSpeedA.value = speedCurve(speed);
    await writeMotorA(realSpeedA.value)
    mutex.release();
  });
});

const motorBDebounce = new Debounce();
watch(motorBSpeed, async (speed) => {
  motorBDebounce.call(async () => {
    await mutex.acquire();
    realSpeedB.value = speedCurve(speed);
    await writeMotorB(realSpeedB.value)
    mutex.release();
  });
});

function onJoystickMove(event: JoystickComponent.UpdateEvent) {
  servoAngle.value = Math.round((event.x ?? 0) * 90);
  motorASpeed.value = Math.round((event.y ?? 0) * 100);
}

const square = JoystickComponent.Shape.Square;

watch(autoReturnZero, (newVal) => {
  if (newVal) {
    servoAngle.value = 0;
    motorASpeed.value = 0;
    motorBSpeed.value = 0;
  }
});
</script>

<template>
  <main>
    <div style="padding: 0 10px;">
      <!-- <h3 style="display: inline-block;">控制面板</h3> -->
      <t-button @click="connect" theme="primary" v-if="!connected">连接</t-button>
      <t-button v-else @click="useServer().disconnect()" theme="danger">断开连接</t-button>
    </div>
    <div style="height: 10px;"></div>
    <cell title="连接状态">
      {{ connected ? '已连接' : '连接断开' }}
    </cell>
    <template v-if="connected">
      <cell title="自动返回原点">
        <template #rightIcon>
          <t-switch v-model="autoReturnZero" :defaultValue="true" />
        </template>
      </cell>
      <!-- <cell title="舵机角度">
        {{ servoAngle }}
      </cell>
      <cell title="前进马达速度">
        {{ motorASpeed }}
      </cell> -->
      <cell title="变形马达速度">
        <template #description>
          <slider v-model:value="motorBSpeed" :label="true" :min="-100" :max="100" :disabled="!connected"
            @touchend="{ if (autoReturnZero) motorBSpeed = 0; }" theme="capsule" />
        </template>
      </cell>
      <cell title="控制摇杆">
        <template #description>
          <div class="joystick-container">
            <joystick :throttle="100" :size="150" :baseShape="square" :stickShape="square" :controlPlaneShape="square"
              :sticky="!autoReturnZero" :disabled="!connected" @move="onJoystickMove" @stop="onJoystickMove" />
          </div>
        </template>
      </cell>
    </template>
  </main>
</template>

<style scoped>
.joystick-container {
  display: flex;
  justify-content: center;
  padding: 30px;
}

.joystick-container>* {
  margin: 0 15px;
}
</style>
