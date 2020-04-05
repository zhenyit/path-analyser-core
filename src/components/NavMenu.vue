<template>
  <div>
    <v-app-bar color="deep-purple accent-4" dense dark flat>
      <!-- <v-app-bar-nav-icon></v-app-bar-nav-icon> -->
      <div class="spacer-divider"></div>
      <img src="../assets/logo.png" height="42px" />

      <!-- <v-toolbar-title>PATH ANALYSIS</v-toolbar-title> -->
      <div class="spacer-divider"></div>

      <v-tabs dark background-color="primary" grow>
        <v-tab>
          <span class="text-lowercase">Examples</span>
        </v-tab>

        <v-tab>
          <span class="text-lowercase">Get Started</span>
        </v-tab>
        <v-tab>
          <span class="text-lowercase">Docs</span>
        </v-tab>
        <v-tab>
          <span class="text-lowercase">Github</span>
        </v-tab>
      </v-tabs>
    </v-app-bar>
  </div>
</template>

<script>
export default {
  name: "NavMenu"
};
</script>

<style>
.theme--dark.v-divider {
  border-color: rgba(255, 255, 255, 0.25) !important;
}
.spacer-divider {
  width: 18px;
}

.text-lowercase {
  text-transform: capitalize !important;
  font-size: 15px;
}

.v-tabs-slider {
  color: #ffb928;
  background: #ffb928 !important;
}
.v-tab--active {
  color: #ffb928 !important;
}
</style>