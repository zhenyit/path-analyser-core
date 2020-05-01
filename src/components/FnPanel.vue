<template>
  <div class="fn-panel">
    <v-expansion-panels multiple>
      <v-expansion-panel>
        <v-expansion-panel-header>ABOUT</v-expansion-panel-header>
        <v-expansion-panel-content>This APP's principal functionality, and the one demonstrated in this online version, is to automate structural unit testing by generating test inputs for full coverage of the C function under test. Full coverage can mean all feasible execution paths or k-path coverage which restricts the all-path criterion to paths with at most k consecutive loop iterations. These are available in this online version.</v-expansion-panel-content>
      </v-expansion-panel>
      <v-expansion-panel>
        <v-expansion-panel-header>SETTING PARAMETERS</v-expansion-panel-header>
        <v-expansion-panel-content>
          <v-text-field v-model="email" :rules="emailRules" label="Variable domains" required></v-text-field>
          <v-text-field
            v-model="email"
            :rules="emailRules"
            label="Unquantified preconditions"
            required
          ></v-text-field>
          <v-text-field
            v-model="email"
            :rules="emailRules"
            label="Quantified preconditions"
            required
          ></v-text-field>
          <v-select :items="items" label="Strategy"></v-select>
        </v-expansion-panel-content>
        <div class="btn">
          <v-btn color="#ffb928" large>Run test</v-btn>
        </div>
      </v-expansion-panel>
    </v-expansion-panels>
  </div>
</template>

<script>
export default {
  name: "FnPanel",
  data() {
    return {
      email: ""
    };
  }
};
</script>

<style>
.fn-panel {
  border: 1px solid #fff;
  background-color: rgb(241, 241, 241);
  height: 100%;
  margin-top: 1px;
}
.btn {
  text-align: center;
  margin-bottom: 20px;
}
</style>