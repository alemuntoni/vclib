# VCLib-Render ToDo list

- General
    - [ ] qt headers and sources should be moved into vclib/processing/qt inside this module,
      and should be included only if vclib-processing target is available
- BGFX
  - [ ] assets and shaders should be compiled in headers that are in paths like
    'vclib/assets/' and 'vclib/shaders/' (right now they are in 'assets/' and 'shaders/')
  - [ ] the first window used by the context to create the bgfx backend context should be
    the first window created by the application (right now it is a dummy window)
  - [ ] check crash example 9015 mesh io stl (crashes if the bimba model (m1) is rendered)
  