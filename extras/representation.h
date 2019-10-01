/*

File: representation.h
 
Copyright (C) 2019 FREVO project contributors

Universitaet Klagenfurt licenses this file to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License. You may obtain a
copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License
is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
or implied. See the License for the specific language governing permissions and limitations under
the License.

*/

#ifndef FREVO_REPRESENTATION_H
#define FREVO_REPRESENTATION_H

namespace frevo {

class Representation {

  // resets the representation
  virtual void reset() {}

  // calculates outputs from inputs
  virtual void calculate(float input[], float output[]) {}
};

}  // namespace frevo

#endif // FREVO_REPRESENTATION_H