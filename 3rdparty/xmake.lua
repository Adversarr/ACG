-- Package: Autodiff.
package("autodiff")
  add_deps("cmake")
  -- add_urls("https://github.com/autodiff/autodiff.git")
  set_sourcedir(path.join(os.scriptdir(), "autodiff"))
  on_install(function (package)
    local configs = {}
    table.insert(configs, "-DCMAKE_BUILD_TYPE="  .. (package:debug() and "Debug" or "Release"))
    table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
    table.insert(configs, "-DAUTODIFF_BUILD_TESTS=OFF")
    table.insert(configs, "-DAUTODIFF_BUILD_PYTHON=OFF")
    table.insert(configs, "-DAUTODIFF_BUILD_EXAMPLES=OFF")
    table.insert(configs, "-DAUTODIFF_BUILD_DOCS=OFF")
    import("package.tools.cmake").install(package, configs)
  end)
package_end()

