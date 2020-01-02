package assimp

foreign import "lib/mesh_loader.lib"

import "core:fmt"

MeshData :: struct {
	vertices: [][3]f32,
	indices: []i32,
}

foreign mesh_loader {
	load_mesh_data :: proc(filename: cstring) -> MeshData ---;
	free_mesh_data :: proc(mesh_data: ^MeshData) ---;
}

main :: proc() {
	mesh_data := load_mesh_data("suzanne.obj");
	fmt.println(len(mesh_data.vertices));
	fmt.println(len(mesh_data.indices));
}