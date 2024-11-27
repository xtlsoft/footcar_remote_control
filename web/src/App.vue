<script setup lang="ts">
import { onMounted, ref, watch } from 'vue';
import { onBeforeRouteUpdate, RouterLink, RouterView, useRoute, useRouter } from 'vue-router'
import { Navbar, TabBar, TabBarItem, Link as TLink } from 'tdesign-mobile-vue'

const router = useRouter();
const currPage = ref('');

onMounted(async () => {
  await router.isReady();
  currPage.value = router.currentRoute.value.path;
});

watch(router.currentRoute, (route) => {
  currPage.value = route.path as string;
});

</script>

<template>
  <main>
    <navbar title="FootCar 小车" :fixed="false" />
    <div style="height: 10px;"></div>

    <RouterView />

    <footer style="text-align: center; margin-top: 20px;">
      <p>
        <small>FootCar Remote Control</small> <br />
        <small>
          <t-link style="display: inline;" hover theme="primary"
            href="https://github.com/xtlsoft/footcar_remote_control">在
            GitHub
            上查看源代码</t-link>
        </small>
      </p>
    </footer>
    <tab-bar theme="tag" :split="false" v-model="currPage" @change="router.push(currPage)">
      <tab-bar-item key="home" value="/">
        控制
      </tab-bar-item>
      <tab-bar-item key="about" value="/about">
        关于
      </tab-bar-item>
    </tab-bar>
  </main>
</template>

<style>
body,
main {
  background-color: #f6f6f6;
  margin: 0;
}
</style>
