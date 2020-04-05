<template>
  <div class="home">
    <!-- <img alt="Vue logo" src="../assets/logo.png"> -->
    <!-- <HelloWorld msg="Welcome to Your Vue.js App"/> -->
    <!-- <h1>This is an about page</h1> -->
    <NavMenu />

    <div class="content-body">
      <v-container class="grey lighten-5" fluid style="height: 100%; padding: 0;">
        <v-row no-gutters style="height: 100%" align="start" align-content="start">
          <v-col cols="4" style="height: 100%">
            <FnPanel />
          </v-col>
          <v-col cols="8" style="height: 100%">
            <!-- CodeMirror Component-->
            <v-row no-gutters style="height: 50%" align="start" align-content="start">
              <v-col cols="12" style="height: 100%">
                <CodeBlock />
              </v-col>
            </v-row>
            <!-- Result Block -->
            <v-row no-gutters style="height: 50%" align="start" align-content="start">
              <v-col cols="12" style="height: 100%" align-self="start">
                <ResultBlock />
              </v-col>
            </v-row>
          </v-col>

          <!-- Navigation Panel -->
        </v-row>
      </v-container>
    </div>
  </div>
</template>

<script>
// @ is an alias to /src
import NavMenu from "@/components/NavMenu.vue";
import FnPanel from "@/components/FnPanel.vue";
import CodeBlock from "@/components/CodeBlock.vue";
import ResultBlock from "@/components/ResultBlock.vue";

export default {
  name: "Home",
  components: {
    NavMenu,
    CodeBlock,
    ResultBlock,
    FnPanel
  }
};
</script>

<style>
.home {
  height: 100%;
}
.content-body {
  height: calc(100% - 48px);
}
</style>
