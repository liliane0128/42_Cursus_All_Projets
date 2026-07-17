import type { ElementType } from "react";
import { useNavigate } from "react-router-dom";

type Props = {
  icon: ElementType;
  label: string;
  active?: boolean;
  to?: string;
};

export default function NavItem({ icon: Icon, label, active = false, to }: Props) {
  const navigate = useNavigate();
  return (
    <button
      onClick={() => to && navigate(to)}
      className={`w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium transition-colors cursor-pointer
        ${active ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-700 hover:bg-gray-50"}`}
    >
      <Icon size={15} strokeWidth={active ? 2.5 : 2} />
      {label}
    </button>
  );
}
