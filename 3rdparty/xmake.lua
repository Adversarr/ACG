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

package("vk-bootstrap")
  set_homepage("https://github.com/charles-lunarg/vk-bootstrap")
  set_description("Vulkan Bootstrapping Iibrary.")
  set_license("MIT")

  add_urls("https://github.com/charles-lunarg/vk-bootstrap/archive/refs/tags/$(version).tar.gz",
           "https://github.com/charles-lunarg/vk-bootstrap.git")

  add_versions("v0.6", "aeed599098060a1ecf6a5e6266748681f7ccc358")
  add_deps("vulkan-headers")

  add_configs("shared", {description = "Build shared library.", default = false, type = "boolean", readonly = true})

  if is_plat("linux") then
      add_syslinks("dl")
  end

  on_install("windows", "linux", "macosx", function (package)
      io.writefile("xmake.lua", [[
          add_rules("mode.debug", "mode.release")
          add_requires("vulkan-headers")
          target("vk-bootstrap")
              set_kind("static")
              set_languages("cxx14")
              add_files("src/VkBootstrap.cpp")
              add_headerfiles("src/VkBootstrap.h")
              add_headerfiles("src/VkBootstrapDispatch.h")
              add_packages("vulkan-headers")
              if is_plat("linux") then
                  add_syslinks("dl")
              end
      ]])
      import("package.tools.xmake").install(package)
  end)

  on_test(function (package)
      assert(package:check_cxxsnippets({test = [[
          vkb::InstanceBuilder builder;
          auto inst_ret = builder.set_app_name ("Example Vulkan Application")
                      .request_validation_layers ()
                      .use_default_debug_messenger ()
                      .build ();
      ]]}, {configs = {languages = "cxx14"}, includes = "VkBootstrap.h"}))
  end)
