<template>
  <div style="height: 100%">
    <codemirror
      ref="cmEditor"
      :value="code"
      :options="cmOptions"
      @ready="onCmReady"
      @focus="onCmFocus"
      @input="onCmCodeChange"
      style="height: 100%"
    />
  </div>
</template>

<script>
import dedent from "dedent";
import { codemirror } from "vue-codemirror";
import "codemirror/lib/codemirror.css";

// language
import "codemirror/mode/clike/clike.js";
// theme css
import "codemirror/theme/mbo.css";

export default {
  name: "CodeBlock",
  components: {
    codemirror
  },
  data() {
    return {
      code: dedent`
    int Tritype(double i, double j, double k) {
        int trityp = 0;
        if (i < 0.0 || j < 0.0 || k < 0.0)
            return 3;
        if (i + j <= k || j + k <= i || k + i <= j)
            return 3;    
        if (i == j) trityp = trityp + 1;
        if (i == k) trityp = trityp + 1;
        if (j == k) trityp = trityp + 1;
        if (trityp >= 2)
            trityp = 2;
        return trityp;
}
`,
      cmOptions: {
        tabSize: 4,
        mode: "text/x-c++src",
        theme: "mbo",
        lineNumbers: true,
        line: true
        // more CodeMirror options...
      }
    };
  },
  methods: {
    onCmReady(cm) {
      console.log("the editor is readied!", cm);
    },
    onCmFocus(cm) {
      console.log("the editor is focused!", cm);
    },
    onCmCodeChange(newCode) {
      console.log("this is new code", newCode);
      this.code = newCode;
    }
  },
  computed: {
    codemirror() {
      return this.$refs.cmEditor.codemirror;
    }
  },
  mounted() {
    console.log("the current CodeMirror instance object:", this.codemirror);
    // you can use this.codeSmirror to do something...
  }
};
</script>

<style>
.CodeMirror {
  border: 1px groove #eee;
  height: 100%;
}
</style>