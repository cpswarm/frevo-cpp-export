/**
 * File: CppExporter.java
 * 
 * Copyright (C) 2019 FREVO project contributors
 *
 * Universitaet Klagenfurt licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License. You may obtain a
 * copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

package at.aau.frevo.exporter.cppexporter;

import org.stringtemplate.v4.STGroupDir;
import at.aau.frevo.Representation;
import at.aau.frevo.representation.fullymeshednet.FullyMeshedNet;

/**
 * Exports {@link Representation} instances as C++ code.
 * <p>
 * Currently only supports {@link FullyMeshedNet}.
 */
public class CppExporter {

  protected boolean enabled;

  /**
   * Creates a {@code CppExporter} instance with the specified parameters.
   * 
   * @param enabled set to enable code generation
   */
  public CppExporter(boolean enabled) {
    this.enabled = enabled;
  }

  /**
   * Converts the specified {@code Representation} instance to code.
   * 
   * @param representation the {@code Representation} instance
   * @return the C++ code
   */
  public String toCode(Representation representation) {
    if (!enabled) {
      return "/* codegen disabled */";
    }

    if (representation instanceof FullyMeshedNet) {
      return fullyMeshedNetToCode((FullyMeshedNet) representation);
    } else {
      return "/* unknown representation type */";
    }
  }


  /**
   * Converts the specified {@code FullyMeshedNet} instance to code.
   * 
   * @param fullyMeshedNet the {@code FullyMeshedNet} instance
   * @return the C++ code
   */
  protected String fullyMeshedNetToCode(FullyMeshedNet fullyMeshedNet) {

    var group = new STGroupDir("at/aau/frevo/exporter/cppexporter");
    var template = group.getInstanceOf("FullyMeshedNet");

    template.add("iterationCount", fullyMeshedNet.getIterationCount());
    template.add("inputCount", fullyMeshedNet.getInputCount());
    template.add("outputCount", fullyMeshedNet.getOutputCount());
    template.add("hiddenNodeCount", fullyMeshedNet.getHiddenNodeCount());
    template.add("biases", fullyMeshedNet.getBiases());
    template.add("weights", fullyMeshedNet.getWeights());
    template.add("randomBiases", fullyMeshedNet.getRandomBiases());

    switch (fullyMeshedNet.getActivationFunction()) {
      case SIGMOID:
        template.add("activationFunction", "ActivationFunction::kSigmoid");
        break;

      case TANH:
        template.add("activationFunction", "ActivationFunction::kTanh");
        break;

      case RELU:
      default:
        template.add("activationFunction", "ActivationFunction::kRelu");
        break;
    }

    return template.render();
  }

  /**
   * Gets the enabled flag.
   * 
   * @return the enabled flag
   */
  public boolean isEnabled() {
    return enabled;
  }

}
