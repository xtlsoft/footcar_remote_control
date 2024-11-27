<script setup lang="ts">
import { h } from 'vue';
import { Empty as TEmpty } from 'tdesign-mobile-vue';
import { InfoCircleFilledIcon } from 'tdesign-icons-vue-next';

const icon = () => h(InfoCircleFilledIcon);
</script>

<template>
  <t-empty :icon="icon" description="暂无简介" />
</template>
