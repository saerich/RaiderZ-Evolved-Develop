* px_rigidbodyproperties.ms ����

Line 180 :

[������]
if (mass_ui.value == 0.0 and density_ui.value == 0.0) then
(
	mass_ui.value = 1.0;
)

[������]
if (mass_ui.value == 0.0 and density_ui.value == 0.0) then
(
	mass_ui.value = 0.0; --1.0;
	density_ui.value = 1.0;
)
